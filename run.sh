#!/bin/bash
qemu-system-x86_64 -monitor stdio -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
