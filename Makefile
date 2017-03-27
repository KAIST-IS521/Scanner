CFLAGS = -Wall
LIBS = -lyara
BUILDDIR = build

all: $(BUILDDIR) $(BUILDDIR)/scanner

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/scanner: scanner.c $(BUILDDIR)
	$(CC) $(CFLAGS) -o$@ $< $(LIBS)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
