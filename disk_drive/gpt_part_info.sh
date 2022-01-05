#!/usr/bin/bash

# 2022-01-05

# GPT disk partitions info (run as root or use sudo)
# util-linux 2.35+ lsblk support all necessary columns to print
# util-linux 2.36+ fdisk support -x, --list-details

DEVICE="/dev/nvme0n1"   # or change to necessary (no multiple devices)
if [ ! -e "$DEVICE" ]
then
    DEVICE="/dev/sda"
fi
echo "DEVICE="$DEVICE

FOUT=partitions_$(hostname)_$(hostid)_$(date -I).txt
NLINE="\n[root]#"
UTILVER="rpm -qf $(which lsblk) $(which fdisk) $(which gdisk)"

CMD1="lsblk -p -o NAME,SIZE,TYPE,FSTYPE,FSVER,LABEL,MOUNTPOINT,PTTYPE,PARTTYPE,PARTTYPENAME,PARTLABEL,PARTFLAGS $DEVICE"

## fdisk -l => Device,Start,End,Sectors,Size,Type
## fdisk -x => Device,Start,End,Sectors,Type-UUID,UUID,Name,Attrs
#CMD2="fdisk -x -o Device,Start,End,Sectors,Size,Type,Type-UUID,Name,Attrs $DEVICE"
## column 'Size' with "-x" print sizes in bytes and with "-l" in powers (MiB, GiB)

# header from "-x" and info from "-l"
CMD2A="fdisk -x $DEVICE | grep -B 999 '^$'"                              # before empty (new) line
CMD2B="fdisk -l -o +Type-UUID,Name,Attrs $DEVICE | grep -A 999 'Device'" # after 'Device' word

CMD3="gdisk -l $DEVICE"

echo "# $(date -I)" > $FOUT
echo -e "$NLINE $UTILVER" >> $FOUT
$UTILVER >> $FOUT

echo -e "$NLINE $CMD1" >> $FOUT
$CMD1 >> $FOUT
#echo -e "$NLINE $CMD2" >> $FOUT
#$CMD2 >> $FOUT
echo -e "$NLINE $CMD2A && $CMD2B" >> $FOUT
(eval $CMD2A && eval $CMD2B) >> $FOUT
echo -e "$NLINE $CMD3" >> $FOUT
$CMD3 >> $FOUT
echo -e "$NLINE" >> $FOUT
