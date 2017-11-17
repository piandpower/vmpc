/**

 \page intro Introduction to IPlug

IPlug was originally developed and maintained by Cockos Inc.  see http://forum.cockos.com/showthread.php?t=952, as part of WDL.
It is a portable Windows/Mac C++ toolkit permitting to develop Audio apps and vst,au,rtas, aax plugins with the same source code base.
It rapidly evolved since and is now maintained by a bunch of enthusiasts developers like Oli Larkin, Tale and others.
GitHub Oli branch can be found at http://github.com/olilarkin/wdl-ol/tree/master/WDL/IPlug

\section intro_features Features

IPlug was designed to be statically linked. This was done by
splitting it into many small objects and designing it so that
functions that are not used do not have pointers to them in the
parts that are used, and thus do not get linked in.

Here are some of the core features unique to IPlug:

\li Develop easily cross platform (Windows, Mac OS X and iOS) code.

\li The same IPlug generic main class can build into several different plug-in formats and a standalone app.

\li Support for advanced & portable GUI graphics.

\li Simple widgets make quick prototyping iPlug plug-ins much easier than with various original plug-in libs.

\li Easier debugging made possible with a standalone app build before testing plugin in OTS audio host applications.

\section intro_licensing Licensing

This version of WDL/IPlug shares the same license as the Cockos edition.
Several of the added features are based on the work of other people.
See individual source code files for any extra license information.

Cockos WDL Page: http://www.cockos.com/wdl
Discuss WDL on the WDL forum http://forum.cockos.com/forumdisplay.php?f=32
---------------------------------------------------------------------------
Cockos WDL License
Copyright (C) 2005 and later Cockos Incorporated
Portions copyright other contributors, see each source file for more information
This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgement in the product documentation
   would be appreciated but is not required.
2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

WDL includes the following 3rd party libraries (which are all similarly licensed):

* JNetLib http://www.nullsoft.com/free/jnetlib
* LibPNG http://www.libpng.org/pub/png
* GifLib http://sourceforge.net/projects/libungif
* JPEGLib http://www.ijg.org
* zlib http://www.zlib.net


\section intro_osx Building and Installing iPlug with Apple OS X

\code
cmake -G Xcode
\endcode

\section intro_windows Building and Installing iPlug with Windows

\code
cmake -G "Visual Studio 10"
\endcode

\section intro_internet Other Resources

\par WWW
http://www.taletn.com/wdl/ [Quick start from Tale] <br>
http://www.martin-finke.de/blog/tags/making_audio_plugins.html [Making Audio Plugins by Martin Finke]<BR>

\par Forums
http://forum.cockos.com/showthread.php?t=87309 [Getting started]<BR>
http://forum.cockos.com/showthread.php?t=91811 [wdl-ol main discussion thread]<BR>

\htmlonly
<hr>
<table summary="navigation bar" width="100%" border="0">
<tr>
  <td width="45%" align="LEFT">
    <a class="el" href="index.html">
    [Prev]
    Main Page
    </a>
  </td>
  <td width="10%" align="CENTER">
    <a class="el" href="index.html">[Index]</a>
  </td>
  <td width="45%" align="RIGHT">
    <a class="el" href="basics.html">
    Basics
    [Next]
    </a>
  </td>
</tr>
</table>
\endhtmlonly

*/
