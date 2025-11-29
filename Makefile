CC = gcc
CFLAGS = -Wall -lpthread

P1 = Partie\ 1
P2 = Partie\ 2

LIST1 = $(P1)/philosophers $(P1)/producer_consumer $(P1)/reader_writer
LIST2 = $(P2)/test1 $(P2)/test2 $(P2)/philosophers_spin $(P2)/producer_consumer_spin $(P2)/reader_writer_spin

all: $(LIST1) $(LIST2)

# Compilation P1
$(P1)/philosophers: $(P1)/philosophers.c
	$(CC) -o "$@" "$<" $(CFLAGS)

$(P1)/producer_consumer: $(P1)/producer_consumer.c
	$(CC) -o "$@" "$<" $(CFLAGS)

$(P1)/reader_writer: $(P1)/reader_writer.c
	$(CC) -o "$@" "$<" $(CFLAGS)


# Compilation P2
$(P2)/test1: $(P2)/spinlocks_performance.c $(P2)/spinlocks.c
	$(CC) -o "$@" $(P2)/spinlocks_performance.c $(P2)/spinlocks.c $(CFLAGS)

$(P2)/test2: $(P2)/spinlocks2_performance.c $(P2)/spinlocks2.c
	$(CC) -o "$@" $(P2)/spinlocks2_performance.c $(P2)/spinlocks2.c $(CFLAGS)

$(P2)/philosophers_spin: $(P2)/philosophers_spin.c $(P2)/spinlocks2.c
	$(CC) -o "$@" $(P2)/philosophers_spin.c $(P2)/spinlocks2.c $(CFLAGS)

$(P2)/producer_consumer_spin: $(P2)/producer_consumer_spin.c $(P2)/spinlocks2.c $(P2)/my_semaphore.c
	$(CC) -o "$@" $(P2)/producer_consumer_spin.c $(P2)/spinlocks2.c $(P2)/my_semaphore.c $(CFLAGS)

$(P2)/reader_writer_spin: $(P2)/reader_writer_spin.c $(P2)/spinlocks2.c $(P2)/my_semaphore.c
	$(CC) -o "$@" $(P2)/reader_writer_spin.c $(P2)/spinlocks2.c $(P2)/my_semaphore.c $(CFLAGS)

# Commandes
test: all
	chmod +x $(P1)/script.sh $(P2)/script2.sh
	cd $(P1) && ./script.sh
	cd $(P1) && python3 philo_plots.py && python3 pc_plots.py && python3 rw_plots.py
	cd $(P2) && ./script2.sh
	cd $(P2) && python3 tests_plot.py
	cd $(P2) && python3 philo_cmp.py && python3 pc_cmp.py && python3 rw_cmp.py
clean:
	rm -f $(LIST1) $(LIST2)
	rm -f $(P1)/analyses/*.csv $(P1)/*.pdf
	rm -f $(P2)/analyses2/*.csv $(P2)/*.pdf

.PHONY: all test clean
