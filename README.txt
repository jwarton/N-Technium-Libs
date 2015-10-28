git@github.com:jwarton/OpenGL-Projects.git
git@github.com:jwarton/N-Technium-Libs.git

Directions for new workstation to access git repositories
https://help.github.com/articles/generating-ssh-keys/#platform-windows

//
//USER CONFIGURATION
$ git config --global user.name "James Warton"
$ git config --global user.email jwarton@smu.edu

git 
//
//GENERATE NEW SSH KEY

$ ssh-keygen -t rsa -b 4096 -C "jwarton@smu.edu"

//
//Enter file in which to save the key 
(/Users/jwarton/.ssh/id_rsa): [Press enter]

//You'll be asked to enter a passphrase.
Enter passphrase (empty for no passphrase): [Type a passphrase]
# Enter same passphrase again: [Type passphrase again]:

//
//ADD YOUR KEY TO THE ssh-agent

$ ssh-agent -s
$ ssh-add ~/.ssh/id_rsa
$ clip < ~/.ssh/id_rsa.pub		(Copies the contents of the id_rsa.pub file to your clipboard)

//Paste contents to new ssh key in github account settings

//TEST CONNECTION
$ ssh -T git@github.com

//CLONE REPOSITORY

$ git clone <repo> <directory>
cd <local project destination>


git add origin

git git remote add upstream https://github.com/ORIGINAL_OWNER/ORIGINAL_REPOSITORY.git

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

C/C++->General->Additional Include Directories:
	$(AF_PATH)\\include
	..\..\ntechnium
	..\..\ntechnium\src
	..\..\ntechnium\src\kuka
	..\..\ntechnium\libNtechnium\integration
	..\..\ntechnium\libNtechnium\iodata
	..\..\ntechnium\libNtechnium\agents
	..\..\ntechnium\libNtechnium\math
	..\..\ntechnium\libNtechnium\fem
	..\..\ntechnium\libNtechnium\robotics
	..\..\ntechnium\libNtechnium\physics
	..\..\ntechnium\libNtechnium\scene
	..\..\ntechnium\libNtechnium\geometry
	..\..\ntechnium\appNtechnium
	..\..\ntechnium\libsImport\glm
	..\..\ntechnium\libsImport\glfw-3.1.1.bin.WIN64\include
	..\..\ntechnium\libsImport\glfw-3.1.1.bin.WIN64\include\GLFW
	..\..\ntechnium\libsImport\glfw-3.0.4.bin.WIN64\include
	..\..\ntechnium\libsImport\glfw-3.0.4.bin.WIN64\include\GLFW
	..\..\ntechnium\libsImport\glew-1.10.0-win32\glew-1.10.0\include
	..\..\ntechnium\libsImport\FreeImage.WIN32\Dist
	..\..\ntechnium\libsImport\freetype-2.5.3\include
	..\..\ntechnium\libsImport\armadillo-5.600.2\include

Linker->Additional Dependiencies->
	afcuda.lib
	afopencl.lib
	afcpu.lib
	glfw3.lib
	glu32.lib
	glew32.lib
	opengl32.lib
	freeImage.lib
	freetype.lib
	blas_win64_MT.lib
	lapack_win64_MT.lib

Linker->Gereral->Additional Library Directories:
	$(AF_PATH)\lib
	..\..\ntechnium\libsImport\armadillo-5.600.2\examples\lib_win64
	..\..\ntechnium\libsImport\freetype-2.5.3\lib
	..\..\ntechnium\libsImport\glew-1.10.0-win32\glew-1.10.0\lib\include
	..\..\ntechnium\libsImport\glew-1.10.0-win32\glew-1.10.0\lib\include\GL
	..\..\ntechnium\libsImport\glew-1.10.0-win32\glew-1.10.0\bin\Release\x64
	..\..\ntechnium\libsImport\glew-1.10.0-win32\glew-1.10.0\lib\Release\x64
	..\..\ntechnium\libsImport\FreeImage.WIN32\Dist
	..\..\ntechnium\libsImport\glfw-3.0.4.bin.WIN64\lib-msvc110
	..\..\ntechnium\libsImport\glfw-3.1.1.bin.WIN64\lib-vc2013

Armadillo Dependicies unresolved.  

COPY THESE TO PROJECT LOCATION:
C:\Users\jwarton\00_Workspace\01_NTlibs\NT-Template\nt-project template\NT-template

blas_win64_MT.dll
lapack_win64_MT.dll

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Add Environment Paths to Advanced System Settings
https://github.com/arrayfire/arrayfire/wiki/Using-ArrayFire-on-Microsoft-Windows-with-Visual-Studio

1. Append Existing Path with ;%AF_PATH%
2.  Add New Path Variable 
		variable name:  AF_PATH
		variable value: C:\Users\jwarton\00_Workspace\01_NTlibs\NT-Template\ntechnium\libsImport\ArrayFire\v3

3.  VS Project Properties 
		C/C++  	General	|  Additional Include Directories

			$(AF_PATH)\include;

		Linker  	General	|	Additional Include Directories:

			$(AF_PATH)\lib;

		Input	|	Additional Include Directories:		
			afcpu.lib;

4.  ArrayFire installation process:  http://arrayfire.com/docs/using_on_windows.htm

When installing or updating AF to:
C:\Users\jwarton\00_Workspace\01_NTlibs\NT-Template\ntechnium\libsImport\ArrayFire

Ensure correct version of CUDA is installed and environment variables are correct:

Copy nvvm64_30_0.dll from:
CUDA_PATH\nvvm\bin\nvvm64_30_0.dll

 to:
 C:\Users\jwarton\00_Workspace\01_NTlibs\NT-Template\nt-project template\x64\Release
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
3dCONNEXION SUPPORT SETUP

1.  Add New Path Variable 
		variable name:  TDX_PATH
		variable value: C:\Program Files (x86)\3Dconnexion\3DxWare SDK

3.  VS Project Properties 
		C/C++  	General	|  Additional Include Directories

			$(TDX_PATH)\inc;

		Linker  	General	|	Additional Include Directories:

			$(TDX_PATH)\lib\x64

		Input	|	Additional Include Directories:	

			siapp.lib
			spwmath.lib

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AF changes from v3.beta to v3

dim_type no longer supported, replaced with dim_t   ////////GGGGRRRRRR

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
update glfw:
glfw-3.0.4 to 3.1.1
..\..\ntechnium\libsImport\glfw-3.0.4.bin.WIN64\lib-msvc110

