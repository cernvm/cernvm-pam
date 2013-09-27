
all: pam_cernvm.so
install: \
  $(DESTDIR)/lib64/security/pam_cernvm.so \
  $(DESTDIR)/etc/cernvm/pam_action.sh

pam_cernvm.o: pam_cernvm.c
	gcc -fPIC -c pam_cernvm.c

pam_cernvm.so: pam_cernvm.o
	gcc -shared -o pam_cernvm.so pam_cernvm.o -lpam

$(DESTDIR)/lib64/security/pam_cernvm.so: pam_cernvm.so pam_action.sh
	mkdir -p $(DESTDIR)/lib64/security
	cp pam_cernvm.so $(DESTDIR)/lib64/security/pam_cernvm.so
	mkdir -p $(DESTDIR)/etc/cernvm
	cp pam_action.sh $(DESTDIR)/etc/cernvm/pam_action.sh

clean:
	rm -f *.o *.so
