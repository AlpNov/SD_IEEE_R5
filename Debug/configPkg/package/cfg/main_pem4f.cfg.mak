# invoke SourceDir generated makefile for main.pem4f
main.pem4f: .libraries,main.pem4f
.libraries,main.pem4f: package/cfg/main_pem4f.xdl
	$(MAKE) -f D:\Documents\School\Sr_Design\CCS_Workspace\embedded_Program/src/makefile.libs

clean::
	$(MAKE) -f D:\Documents\School\Sr_Design\CCS_Workspace\embedded_Program/src/makefile.libs clean

