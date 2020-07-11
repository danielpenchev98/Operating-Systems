#!/bin/bash

#Daniel Svetoslavov Penchev SI gr2 62114

#checking the number of arguments
if [ "${#}" -ne 4 ]; then
        echo "Invalid number of arguments.";
        exit 1;
fi

#initializing variables with the values of arguments for better readability
readonly logFile="${1}";
readonly eegFile="${2}";
readonly wavFile="${3}";
readonly targetDir="${4}";

#checking whether any of the files arent readable/exist or the target directory isnt writable/exist
if [ ! -f "${logFile}"  ]||[ ! -r "${logFile}" ];then
        echo "Error the log file isn't a file or cannot be found or isn't readable";
        exit 2;
elif [ ! -f "${eegFile}" ]||[ ! -r "${eegFile}" ];then
        echo "Error the eeg file isn't a file or cannot be found or isn't readable";
        exit 3;
elif [ ! -f "${wavFile}"  ]||[ ! -r "${wavFile}" ]; then
        echo "Error the wav file isn't a file or cannot be found or isn't readable";
        exit 4;
elif [ ! -w "${targetDir}" -o ! -x "${targetDir}" -o "$( stat -c %F "${targetDir}" )" != "directory" ]; then
        echo "Error the target directory cannot be found or isn't writable and executable";
	exit 5;
fi

#checking the timezone
if [ -z "$EEG_TZ" ]; then
	EEG_TZ='UTC'
fi

##### User defined functions #######

#function for calculating the time in seconds
calcTimeSec() {
     if [ "${#}" -ne 1 ]; then
	     echo "The fuction calcTimeSec() needs 1 argument exacly";
	     exit 6;
     fi
     local result="$(echo "${1}" | awk -F':' '{ cmd=" echo " ( $1 * 3600 )  + ( $2 * 60 ) + $3 " | bc " ; cmd | getline result ; print result }')"
     echo "${result}";
}

####################################

#Getting the date of the record, the start time of the record, the length of the record, and the samplingrate from eeg file, removing the tags
readonly extract="$(cat "${eegFile}" | egrep "^<StartRecordingDate>|^<StartRecordingTime>|^<RecordLength>|^<SamplingRate>" | sort -u  -t '>' -k1,1 \
           | sed -r 's/[^>]*>([^<>]+)<.*/\1/')" 

#The value of extract will be:
#       1) RecordLength
#       2) SamplingRate
#       3) StartRecordingDate
#       4) StartRecordingTime


#if there where duplicate tags in extract, then the program will exit
if [ "$(echo "${extract}" | wc -l)" -ne 4 ]; then
	echo "Error, one of the following elements from the xml file is missing or there are duplicates of these elements :StartRecordingDate, StartReconrdingTime, RecordLength or SamplingRate "
	exit 7;
fi

#calculating the starting time of the eeg
#the date must be in the format "2019-02-19"
readonly startTime="$( TZ="${EEG_TZ}" date -d "$( echo "${extract}" | head -n 3 | tail -n 1 | sed 's/\./-/g' \
              | awk -F'-' '{ print $3"-"$2"-"$1 }' )  $( echo "${extract}" | tail -n 1 | tail -n 1 )" +%s.%6N )"

#EEG_File variables
readonly lengthEEG="$(calcTimeSec "$(echo "${extract}" | head -n  1)")"
readonly rateOfTicks="$( echo "${extract}" | head -n 2 | tail -n 1 | cut -d ' ' -f1)"
readonly endTime="$(echo "${startTime} + ${lengthEEG}" | bc )"

# gonna use soxi instead of $(ffmpeg -i wavFile | grep Duration | cut -f ' ' -f4 | sed 's/,//') + calcTimeSec() because i get more accurate result
#WAV_File variables
readonly lengthWav="$(soxi -D "${wavFile}")"
readonly wavBegin="$( cat "$logFile" | egrep "^beep" | cut -d ' ' -f3)"
readonly wavEnd="$( echo "${lengthWav} + ${wavBegin}" | bc )"


#Warning the user for diplicate stimulus
cat "${logFile}" | cut -d ' ' -f1 | sort | uniq -c | tr -s ' ' | cut -d ' ' -f2- \
       | awk -F' ' '{ if($1 != 1){ print "Error, stimulus "$2" has  duplicates"} }'

#filtering the records
readonly legitStim="$(cat "${logFile}"  | awk -F' ' -v eegB="${startTime}" -v eegE="${endTime}" -v wavB="${wavBegin}" -v wavE="${wavEnd}" \
             '{ cmd="echo "$3 - $2" | bc" ; cmd | getline result; 
	        if( result < 0.2 ) 
                { 
		   print "Error: the record of "$1" is less than 0.2"
                } 
	        else if( $2 < eegB || $3 > eegE || $2 < wavB || $3 > wavE  )
		{
		   print "Error: the record of "$1" is out of range"
		}
                else{ print $0 }   
	      }' )"

#changing the separator to newline only
readonly temp="${IFS}"
IFS=$'\n'

#iterating through the the list of records
for stim in $(echo "${legitStim}"  ); do

	#checking if we should skip the record
        if [[ "${stim}" =~ ^Error: ]]; then
	    echo "${stim}";
	    continue;
	fi

        #calculating the start, end and length of the record
	startStim="$(echo "${stim}" | cut -d ' ' -f2 )"
        endStim="$(echo "${stim}" | cut -d ' ' -f3 )"
        lengthStim="$( echo "${endStim} - ${startStim}" | bc )"

	#calculating the number of ticks to skip and the number of ticks to get
	#all numbers are cut after the floaring point
	ticksStim="$( echo " ${lengthStim} * ${rateOfTicks}" | bc | sed -r 's/\..*//' )"
	firstTick="$( echo "(( ${startStim} - ${startTime}) * ${rateOfTicks}) +1" | bc | sed -r 's/\..*//' )"
	
	#cutting the wav file.  sox sourceFile targetFile trim theStartingMomentInTheWavFile length of the record
	sox "${wavFile}" "${targetDir}/$( echo "${stim}" | cut -d ' ' -f1)_lar.wav" trim "$( echo "${startStim} - ${wavBegin}" | bc )" "${lengthStim}"
	#getting the ticks for every single stimulus
	cat "${eegFile}" | egrep "^<tick>" | tail -n "+${firstTick}" | head -n "${ticksStim}" > "${targetDir}/$( echo "${stim}" | cut -d ' ' -f1)_eeg.xml" 
done
IFS="${temp}"
