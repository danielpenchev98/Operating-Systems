#!/bin/bash

#62114 - program which compares a word from text.txt with the words from dic.txt#in order to find the best possible matching word( the levenstein's length to be#minimal but greater than 0)

#important files, which the program uses 
txtFile="./text.txt"
dictionary="./dic.txt"

#checking if the files exist and are readable
if [[ ! -r "$txtFile" ]] || [[ ! -r "$dictionary" ]]; then

	#sending the error message to stderr
	echo "Critical error. A file is missing or isnt readable" >&2
	exit 1
fi

# using a for loop to cycle through every single word from text.txt
# format of the list( for i  [in list];then ...) is : word1 word2 word3 ...
# all words in the list is unique
for i in $(cat "$txtFile" | sed 's/[A-ZА-Я]/\L&/g' | sed 's/[^a-zа-я]/\n/g'| grep -v '^$'| sort | uniq) ; do

    # saving the result of the approx match between a word from the list and a      # word from the dictionary
    
    result=$(tre-agrep -Bsw -E 99 "$i" "$dictionary" |head -n 1 )

    #checking if its a full match or not
         if (( $(echo "$result" | cut -d ":" -f1) > 0 )); then

    #printing format is - word from textFile:Levenstein length:word from dic.txt
                echo "$i:$result"
         fi
done
