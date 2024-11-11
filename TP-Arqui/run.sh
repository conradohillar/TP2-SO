if [ "$1" = "-d" ]; then
	qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 
	# -m 512 -audiodev alsa,id=speaker,out.dev=default -machine pcspk-audiodev=speaker
else
	qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 
	# -m 512 -audiodev alsa,id=speaker,out.dev=default -machine pcspk-audiodev=speaker
fi 

