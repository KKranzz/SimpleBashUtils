#!/bin/bash
echo "TEST GREP BY VALUE"
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
	TEST2="$var $FILE1 -e $REGEX1 -e $REGEX2"
	echo "$TEST1"
	$S21_GREP $TEST1 > s21_grep.txt
	grep $TEST2 > grep.txt
	sha1_file1=$(shasum "s21_grep.txt" | cut -d' ' -f1)
	sha1_file2=$(shasum "grep.txt" | cut -d' ' -f1)
	if [ "$sha1_file1" == "$sha1_file2" ]; then
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
	TEST1="$var $var2 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE2 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
	TEST2="$var $var2 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE2 $FILE1 $FILE1 $FILE1 $FILE1 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
	echo "$TEST1"
	$S21_GREP $TEST1 > s21_grep.txt
	grep $TEST2 > grep.txt
	sha1_file1=$(shasum "s21_grep.txt" | cut -d' ' -f1)
	sha1_file2=$(shasum "grep.txt" | cut -d' ' -f1)
	if [ "$sha1_file1" == "$sha1_file2" ]; then
		(( COUNTER_OK++ ))
	else
 		(( COUNTER_FAIL++ ))
		echo "$TEST1" >> errors.txt
	fi
	done
done

for var in -v -c -l -n -i;
do
	for var2 in -v -c -l -n -i
	do
		for var3 in -v -c -l -n -i
		do
			TEST1="$var $var2 $var3 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
			TEST2="$var $var2 $var3 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
			echo "$TEST1"
			$S21_GREP $TEST1 > s21_grep.txt
			grep $TEST2 > grep.txt
			sha1_file1=$(shasum "s21_grep.txt" | cut -d' ' -f1)
			sha1_file2=$(shasum "grep.txt" | cut -d' ' -f1)
			if [ "$sha1_file1" == "$sha1_file2" ]; then
				(( COUNTER_OK++ ))
			else
 				(( COUNTER_FAIL++ ))
				echo "$TEST1" >> errors.txt
			fi
		done
	done
done

for var4 in -v -c -l -n -i;
do
	for var in -v -c -l -n -i;
	do
		for var2 in -v -c -l -n -i
		do
			for var3 in -v -c -l -n -i
			do
				TEST1="$var $var2 $var3 $var4 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
				TEST2="$var $var2 $var3 $var4 $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
				echo "$TEST1"
				$S21_GREP $TEST1 > s21_grep.txt
				grep $TEST2 > grep.txt
				sha1_file1=$(shasum "s21_grep.txt" | cut -d' ' -f1)
				sha1_file2=$(shasum "grep.txt" | cut -d' ' -f1)
				if [ "$sha1_file1" == "$sha1_file2" ]; then
					(( COUNTER_OK++ ))
				else
 					(( COUNTER_FAIL++ ))
					echo "$TEST1" >> errors.txt
				fi
			done
		done
	done
done
				
				echo "-vclni $FILE1 $FILE2 -e $REGEX1 -e $REGEX2"
				$S21_GREP -vclni $FILE1 $FILE2 -e $REGEX1 -e $REGEX2> s21_grep.txt
				grep -vclni $FILE1 $FILE2 -e $REGEX1 -e $REGEX2 > grep.txt
				sha1_file1=$(shasum "s21_grep.txt" | cut -d' ' -f1)
				sha1_file2=$(shasum "grep.txt" | cut -d' ' -f1)
				if [ "$sha1_file1" == "$sha1_file2" ]; then
					(( COUNTER_OK++ ))
				else
 					(( COUNTER_FAIL++ ))
					echo "-vclni $FILE1 $FILE2 -e $REGEX1 -e $REGEX2" >> errors.txt
				fi

echo "OK: $COUNTER_OK"
echo "FAIL: $COUNTER_FAIL"
