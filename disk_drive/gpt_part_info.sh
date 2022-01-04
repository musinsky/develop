#!/usr/bin/bash

# 2022-01-04
# disk (only GPT) partitions info

DEVICE="/dev/nvme0n1"   # no multiple devices
if [ ! -e "$DEVICE" ]
then
    DEVICE="/dev/sda"
fi
echo "DEVICE="$DEVICE

FOUT=partitions_$(hostname)_$(hostid)_$(date -I).txt
NLINE="\n[root]#"
UTILVER="rpm -qf $(which lsblk) $(which fdisk) $(which gdisk)"

CMD1="lsblk -p -o NAME,SIZE,TYPE,FSTYPE,FSVER,LABEL,MOUNTPOINT,PTTYPE,PARTTYPE,PARTTYPENAME,PARTLABEL,PARTFLAGS $DEVICE"
CMD2="fdisk -l $DEVICE"
CMD3="fdisk -x $DEVICE"
CMD4="gdisk -l $DEVICE"
# header from "fdisk -x" and info from "fdisk -l"
CMD2A="fdisk -x $DEVICE | grep -B 999 '^$'"                              # before empty (new) line
CMD2B="fdisk -l -o +Type-UUID,Name,Attrs $DEVICE | grep -A 999 'Device'" # after 'Device' word


echo "# $(date -I)" > $FOUT
echo -e "$NLINE $UTILVER" >> $FOUT
$UTILVER >> $FOUT

echo -e "$NLINE $CMD1" >> $FOUT
$CMD1 >> $FOUT
#echo -e "$NLINE $CMD2" >> $FOUT
#$CMD2 >> $FOUT
#echo -e "$NLINE $CMD3" >> $FOUT
#$CMD3 >> $FOUT
echo -e "$NLINE $CMD2A && $CMD2B" >> $FOUT
(eval $CMD2A && eval $CMD2B) >> $FOUT
echo -e "$NLINE $CMD4" >> $FOUT
$CMD4 >> $FOUT
echo -e "$NLINE" >> $FOUT
