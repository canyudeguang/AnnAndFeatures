#!/bin/bash

TestDir=$2
TrainDir=$1
NumCount=10

if [ "$#" -eq 3 ];then
	NumCount=$(($3))
	echo $NumCount
fi


if [ "$#" -ge 2 ];then
	echo $TestDir $TrainDir
	#If Test dir contain any files - move them back
	if [ `ls $TestDir | wc -l` -gt 0 ];then
		echo "Moving back TEST dir: $TestDir with into TRAIN dir: $TrainDir`ls $TestDir | wc -l` files"
		mv $TestDir/*.jpg $TrainDir
	fi

	if [ `ls $TrainDir | wc -l` -gt $NumCount ];then 
		echo "Picking random $NumCount images from TRAIN dir: $TrainDir from `ls $TrainDir | wc -l` samples" 
		for f in `ls $TrainDir | shuf -n $NumCount`;do
			mv $TrainDir/$f $TestDir
		done
		echo "Copying $NumCount from $TrainDir to $TestDir"
	fi
	# Chose random 
fi
