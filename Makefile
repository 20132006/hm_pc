MPICC  = mpicc
MPICXX = mpiic++
MPIFC  = mpiif77

MPICFLAGS = -O3

TARGETS = count3s

all : $(TARGETS)

count3s : count3s.c
	$(MPICC) $(MPICFLAGS) count3s.c -o count3s

clean :
	rm -rf $(TARGETS)
