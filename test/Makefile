OPTIONS=-lgtest -lgtest_main -pthread
encoder_test:
	g++ -o encoder_test encoder_test.cc ../base64_encoder.cc $(OPTIONS)

decoder_test:
	g++ -o decoder_test decoder_test.cc ../base64_decoder.cc $(OPTIONS)

clean_decoder:
	rm decoder_test
