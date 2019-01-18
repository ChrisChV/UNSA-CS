.PHONY : submission1 submission2 clean master
	
submission1:
	tar cvzf submission_part1.tar.gz -C attacks .
	
submission2: clean
	tar cvzf submission_part2.tar.gz -C bitbar .
	
master: clean
	tar cvzf proj2.tar.gz -C ../proj2 .
	
clean:
	rake -f bitbar/Rakefile db:reset
	rake -f bitbar/Rakefile assets:clobber
	rake -f bitbar/Rakefile tmp:clear
