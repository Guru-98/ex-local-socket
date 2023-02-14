CFLAGS += -I./. -g

OBJECTS := frontend.o backend.o
TARGETS := fend bend

fend: frontend.o
	@echo "LD: $< -> $@"
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

bend: backend.o
	@echo "LD: $< -> $@"
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.c
	@echo "CC: $< -> $@"
	$(CC) $(CFLAGS) -c $(CPPFLAGS) $< -o $@

.PHONY: all
.DEFAULT: all
all: fend bend 

.PHONY: clean
clean:
	rm -f $(TARGETS) $(OBJECTS)

