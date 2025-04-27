#!/bin/bash
echo "TEST BY MEMORY FOR GREP"
COUNTER_OK=0
COUNTER_FAIL=0
FILE1="../../src/grep/main_grep.c"
FILE2="../../data-samples/file1.txt"
S21_GREP="../../src/grep/s21_grep"
REGEX1="^#include.*$"
REGEX2="another"
echo "" > errors.txt

pushd ../../src/grep
make
popd

for var in -v -c -l -n -i;
do
	TEST1="$var $FILE1 -e $REGEX1 -e $REGEX2"
	echo "$TEST1"
	valgrind --tool=memcheck --leak-check=yes $S21_GREP $TEST1 > s21_grep.txt 2>&1
	ERROR_COUNT=$(grep -o "ERROR SUMMARY: [0-9]* errors" s21_grep.txt | grep -o "[0-9]*")
	if [ "$ERROR_COUNT" == "0" ]; then
		(( COUNTER_OK++ ))
	else
 		(( COUNTER_FAIL++ ))
		echo "$TEST1" >> errors.txt
	fi
done

for var in -v -c -l -n -i;
do
	for var2 in -v -c -l -n -i
	do
		TEST1="$var $var2 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 -e $REGEX1 -e $REGEX2"
		echo "$TEST1"
		valgrind --tool=memcheck --leak-check=yes $S21_GREP $TEST1 > s21_grep.txt 2>&1
		ERROR_COUNT=$(grep -o "ERROR SUMMARY: [0-9]* errors" s21_grep.txt | grep -o "[0-9]*")
		if [ "$ERROR_COUNT" == "0" ]; then
			(( COUNTER_OK++ ))
		else
 			(( COUNTER_FAIL++ ))
			echo "$TEST1" >> errors.txt
		fi
	done
done

 		TEST1="-vclni $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE2 $FILE2 -e $REGEX1 -e $REGEX2"
                echo "$TEST1"
                valgrind --tool=memcheck --leak-check=yes $S21_GREP $TEST1 > s21_grep.txt 2>&1
                ERROR_COUNT=$(grep -o "ERROR SUMMARY: [0-9]* errors" s21_grep.txt | grep -o "[0-9]*")
                if [ "$ERROR_COUNT" == "0" ]; then
                        (( COUNTER_OK++ ))
                else
                        (( COUNTER_FAIL++ ))
                        echo "$TEST1" >> errors.txt
                fi


echo "OK: $COUNTER_OK"
echo "FAIL: $COUNTER_FAIL"
