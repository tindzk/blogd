#!/bin/bash

mkdir -p Build/{Resources,Jivai,Debit,Domain,docslib}

../tplgen/tplgen.bin              \
	name=Template                 \
	itf=no                        \
	add=Feed:Templates/Feed.xml   \
	add=Body:../docslib/Body.html \
	dir=Templates                 \
	ext=.html                     \
	out=Source/Template           \
	|| exit 1

../Jivai-Utils/jutils.bin         \
	build                         \
	output=blogd.bin              \
	manifest=Manifest.h           \
	main=Source/Main.c            \
	include=Source                \
	include=.                     \
	include=..                    \
	include=../Jivai/src          \
	add=Resources/*.c             \
	map=Source:Build              \
	map=Domain:Build/Domain       \
	map=Resources:Build/Resources \
	map=../Jivai/src:Build/Jivai  \
	map=../Debit:Build/Debit      \
	map=../docslib:Build/docslib  \
	optimlevel=0                  \
	dbgsym=yes                    \
	inclhdr=config.h              \
	link=@dl                      \
	link=@bfd
