![Build status](https://img.shields.io/badge/OpenWSP-0.1-blue.svg)
![Build status](https://img.shields.io/badge/build-passing-green.svg)
![Item status](https://img.shields.io/badge/status-unstable-lightgreen.svg)

# OpenWSP (developing)

<br> an opensource hardware, providing a Web Stream codec/Player framework for embedded systems. <br/>

These pictures show how WSP runs on a 240x320 LCD platform:

![screen](https://raw.githubusercontent.com/opxarch/privdats/master/OpenWSP/320x240-captured-0.JPG)
screen-1 Volume Panel<br/>
![screen](https://raw.githubusercontent.com/opxarch/privdats/master/OpenWSP/320x240-captured-1.JPG)
screen-2 Radio Categories from the Internet<br/>
![screen](https://raw.githubusercontent.com/opxarch/privdats/master/OpenWSP/320x240-captured-2.JPG)
screen-3 Radio Channels<br/>
![screen](https://raw.githubusercontent.com/opxarch/privdats/master/OpenWSP/320x240-captured-3.JPG)
screen-4 Audio Playing<br/>

<br/> &emsp;&emsp;It runs on a variet of platforms, such as `GNU Linux`, `OpenWRT`, `DD-WRT` (yet another Linux cored OS) and `Windows`. It is also compatible with MIPS/ARM/PPC/X86/AMD64 or most of the other processor architectures.
<br/>
<br/> &emsp;&emsp;Having implemented an Webservice interface, we are able to download the information of streams from the Internet, including a list of catalogue, styles and programs. What's more, you can even search the programs in which you're intererted. At present we are trying to access to `Shoutcast`(Winamp) and `QingTing FM` open API.
<br/>
<br/> &emsp;&emsp;Currently we have two available font-ends: `desktop` and `smart terminal`. The desktop version is under development, while the smart version has an μC/GUI user interface, and can be connected with a TFT-LCD device, furthermore, an optional touch-pad driver is being constructed.
<br/>
<br/>

----

## building tools dependency:
sed 4.2.1<br/>
python 2.7<br/>

----

## coding languages:
Native ASM<br/>
ANSI C<br/>
C++<br/>
Javascript<br/>

----

## important:
<br> Some of the files in this project is from MPlayer (C).<br/>
MPlayer was originally written by Árpád Gereöffy.<br/>
Copyright MPlayer term.<br/>
