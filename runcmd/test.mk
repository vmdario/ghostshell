
# How to prepare to run the tests

tests_setup:
	@rm -f nosuchfile *.log

# What to do after tests (eg clean temp files)

tests_cleanup:
	@rm -f t1.log
