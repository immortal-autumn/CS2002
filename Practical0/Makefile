# First target is what is run if you just type make
all: stage1 stage2 stage3

# Clean target will delete all object files
clean:
	rm *.o stage1 stage2 stage3

# Stage1 will link printpell and recursive_spell and run the recursive function which will read from users
# and recursively return the result.
stage1 : print_pell.o recursive_spell.o
	clang print_pell.o recursive_spell.o -o stage1 -Wall -Wextra -g

# Stage2 will link printpell and iterative_pell and run the itertive function which will read from users
# and iteratively return the result.
stage2 : print_pell.o iterative_pell.o
	clang print_pell.o iterative_pell.o -o stage2 -Wall -Wextra -g

# Stage3 is designed for the third requirement of practical and will run the personalised pell which will read
# first and second number from users.
stage3 : second_main.o personalised_pell.o
	clang second_main.o personalised_pell.o -o stage3 -Wall -Wextra -g

# Simply compile the print_pell main function
print_pell.o : print_pell.c pell.h
	clang print_pell.c -c -o print_pell.o -Wall -Wextra -g

# Simply compile the second_main main functions
second_main.o : second_main.c pell.h
	clang second_main.c -c -o second_main.o -Wall -Wextra -g

# Simply compile the recursive_spell function
recursive_spell.o : recursive_spell.c pell.h
	clang recursive_spell.c -c -o recursive_spell.o -Wall -Wextra -g

iterative_pell.o : iterative_pell.c pell.h
	clang iterative_pell.c -c -o iterative_pell.o -Wall -Wextra -g

personalised_pell.o : personalised_pell.c pell.h
	clang personalised_pell.c -c -o personalised_pell.o -Wall -Wextra -g