#!/bin/bash
echo "TEST CAT BY VALUE"
COUNTER_OK=0
COUNTER_FAIL=0
FILE1="../../src/cat/main_cat.c"
FILE2="../../data-samples/bytes.txt"
S21_CAT="../../src/cat/s21_cat"
echo "" > errors.txt

pushd ../../src/cat
make
popd

for var in -b -e -n -v -s -t;
do
	TEST1="$var $FILE2"
	TEST2="$var $FILE2"
	echo "$TEST1"
	$S21_CAT $TEST1 > s21_cat.txt
	cat $TEST2 > cat.txt
        sha1_file1=$(shasum "s21_cat.txt" | cut -d' ' -f1)
	sha1_file2=$(shasum "cat.txt" | cut -d' ' -f1)
	if [ "$sha1_file1" == "$sha1_file2" ]; then
		(( COUNTER_OK++ ))
	else
 		(( COUNTER_FAIL++ ))
		echo "$TEST1" >> errors.txt
	fi
done

for var in -b -e -n -v -s -t;
do
	for var2 in -b -e -n -v -s -t;
	do
	TEST1="$var $var2 $FILE2 $FILE1"
	TEST2="$var $var2 $FILE2 $FILE1"
	echo "$TEST1"
	$S21_CAT $TEST1 > s21_cat.txt
	cat $TEST2 > cat.txt
        sha1_file1=$(shasum "s21_cat.txt" | cut -d' ' -f1)
	sha1_file2=$(shasum "cat.txt" | cut -d' ' -f1)
	if [ "$sha1_file1" == "$sha1_file2" ]; then
		(( COUNTER_OK++ ))
	else
 		(( COUNTER_FAIL++ ))
		echo "$TEST1" >> errors.txt
	fi
	done
done


for var in -b -e -n -v -s -t;
do
	for var2 in -b -e -n -v -s -t;
	do
		for var3 in -b -e -n -v -s -t;
		do
		TEST1="$var $var2 $var3 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
		TEST2="$var $var2 $var3 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
		echo "$TEST1"
		$S21_CAT $TEST1 > s21_cat.txt
		cat $TEST2 > cat.txt
        	sha1_file1=$(shasum "s21_cat.txt" | cut -d' ' -f1)
		sha1_file2=$(shasum "cat.txt" | cut -d' ' -f1)
		if [ "$sha1_file1" == "$sha1_file2" ]; then
			(( COUNTER_OK++ ))
		else
 			(( COUNTER_FAIL++ ))
		echo "$TEST1" >> errors.txt
		fi
		done
	done
done

for var in -b -e -n -v -s -t;
do
	for var2 in -b -e -n -v -s -t;
	do
		for var3 in -b -e -n -v -s -t;
		do
			for var4 in -b -e -n -v -s -t;
			do
			TEST1="$var $var2 $var3 $var4 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
			TEST2="$var $var2 $var3 $var4 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
			echo "$TEST1"
			$S21_CAT $TEST1 > s21_cat.txt
			cat $TEST2 > cat.txt
        		sha1_file1=$(shasum "s21_cat.txt" | cut -d' ' -f1)
			sha1_file2=$(shasum "cat.txt" | cut -d' ' -f1)
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
			TEST1="-benstv $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
			TEST2="-benstv $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
			echo "$TEST1"
			$S21_CAT $TEST1 > s21_cat.txt
			cat $TEST2 > cat.txt
        		sha1_file1=$(shasum "s21_cat.txt" | cut -d' ' -f1)
			sha1_file2=$(shasum "cat.txt" | cut -d' ' -f1)
			if [ "$sha1_file1" == "$sha1_file2" ]; then
				(( COUNTER_OK++ ))
			else
 				(( COUNTER_FAIL++ ))
			echo "$TEST1" >> errors.txt
			fi

echo "OK: $COUNTER_OK"
echo "FAIL: $COUNTER_FAIL"
