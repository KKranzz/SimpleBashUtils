#!/bin/bash
echo "TEST CAT BY MEMORY"
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
	echo "$TEST1"
	valgrind --tool=memcheck --leak-check=yes $S21_CAT $TEST1 > s21_cat.txt 2>&1
	ERROR_COUNT=$(grep -o -a "ERROR SUMMARY: [0-9]* errors" s21_cat.txt | grep -o -a "[0-9]*")
	if [ "$ERROR_COUNT" == "0" ]; then
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
		TEST1="$var $var2 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
		echo "$TEST1"
		valgrind --tool=memcheck --leak-check=yes $S21_CAT $TEST1 > s21_cat.txt 2>&1
		ERROR_COUNT=$(grep -o -a "ERROR SUMMARY: [0-9]* errors" s21_cat.txt | grep -o -a "[0-9]*")
		if [ "$ERROR_COUNT" == "0" ]; then
			(( COUNTER_OK++ ))
		else
 			(( COUNTER_FAIL++ ))
			echo "$TEST1" >> errors.txt
		fi
	done
done

 		TEST1="-benstv $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1 $FILE2 $FILE1"
                echo "$TEST1"
                valgrind --tool=memcheck --leak-check=yes $S21_CAT $TEST1 > s21_cat.txt 2>&1
		ERROR_COUNT=$(grep -o -a "ERROR SUMMARY: [0-9]* errors" s21_cat.txt | grep -o -a "[0-9]*")
		if [ "$ERROR_COUNT" == "0" ]; then
			(( COUNTER_OK++ ))
		else
 			(( COUNTER_FAIL++ ))
			echo "$TEST1" >> errors.txt
		fi

echo "OK: $COUNTER_OK"
echo "FAIL: $COUNTER_FAIL"
