.PHONY: all serverMain serverInternal serverExternal clean

# Build everything by calling sub-Makefiles
all: serverMain serverInternal serverExternal

# Build serverMain
serverMain:
	$(MAKE) -C serverMain

# Build serverInternal
serverInternal:
	$(MAKE) -C serverInternal

# Build serverExternal
serverExternal:
	$(MAKE) -C serverExternal

# Clean everything
clean:
	$(MAKE) -C serverMain clean
	$(MAKE) -C serverInternal clean
	$(MAKE) -C serverExternal clean
