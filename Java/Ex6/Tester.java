

public class Tester {
	private static int numFailed = 0;
	private static int numSuccess = 0;
	
	public static void main(String[] args) {
		LimitedSentence sentence0 = new LimitedSentence('#');
		reportTest(sentence0.getFirstLetter() == 0, 1);
		reportTest(sentence0.getMiddleLetter() == 0, 2);
		reportTest(sentence0.getLastLetter() == 0, 3);
		reportTest(sentence0.getSentence().equals(""), 4); 
		
		LimitedSentence sentence1 = new LimitedSentence(' ');
		sentence1.appendWordEnd("corona");
		sentence1.appendWordFront("hello");
		reportTest(sentence1.getSentence().equals("hello corona"), 5); 
		
		LimitedSentence sentence2 = new LimitedSentence(sentence1);
		reportTest(sentence2.getSentence().equals("hello corona"), 6); 
		
		// test other methods
		reportTest(sentence1.getDelimiter() == ' ', 7);  
		reportTest(sentence1.getFirstLetter() == 'h', 8);
		reportTest(sentence1.getFirstLetter() == 'h', 8);
		reportTest(sentence1.getMiddleLetter() == 'c', 9);
		reportTest(sentence1.getLastLetter() == 'a', 10);
		
		// add words
		reportTest(sentence1.appendWordEnd("ab1") == false, 11);
		reportTest(sentence1.getSentence().equals("hello corona"), 12); 
		
		reportTest(sentence1.appendWordEnd("goodbye") == true, 13);
		reportTest(sentence1.getSentence().equals("hello corona goodbye"), 14); 
		
		reportTest(sentence1.appendWordEnd("good bye") == false, 15);
		reportTest(sentence1.getSentence().equals("hello corona goodbye"), 16); 
		
		reportTest(sentence1.appendWordEnd("thisisaverylongword") == false, 17);
		reportTest(sentence1.getSentence().equals("hello corona goodbye"), 18); 
		
		reportTest(sentence0.appendWordFront("Hello") == true, 19);
		reportTest(sentence0.getSentence().equals("Hello"), 20); 
		
		reportTest(sentence0.appendWordFront("Try#Again") == false, 21);
		reportTest(sentence0.getSentence().equals("Hello"), 22); 
		
		reportTest(sentence0.appendWordFront("World") == true, 23);
		reportTest(sentence0.getSentence().equals("World#Hello"), 24); 
		
		reportTest(sentence0.appendWordEnd("nice") == true, 25);
		reportTest(sentence0.getSentence().equals("World#Hello#nice"), 26);  
		
		reportTest(sentence0.getIthWord(0).equals("World"), 27); 
		reportTest(sentence0.getIthWord(1).equals("Hello"), 28); 
		reportTest(sentence0.getIthWordReversed(0).equals("dlroW"), 29); 
		
		reportTest(sentence0.countWords() == 3, 30); 
		sentence0.reverseUpperLower();
		reportTest(sentence0.toString().
				equals("wORLD#hELLO#NICE"), 31); 
		
		LimitedSentence sentence3 = new LimitedSentence('#'); 
		sentence3.appendWordEnd("hey");
		sentence3.appendWordEnd("man");
		reportTest(sentence3.getMiddleLetter() == '#', 32);
		
		// some more tests
		LimitedSentence sentence4 = new LimitedSentence(' ');
		sentence4.appendWordEnd("aba");
		reportTest(sentence4.getFirstLetter() == 'a', 33);
		reportTest(sentence4.getMiddleLetter() == 'b', 34);
		reportTest(sentence4.getLastLetter() == 'a', 35);
		
		LimitedSentence sentence5 = new LimitedSentence(' ');
		sentence5.appendWordEnd("abcd");
		reportTest(sentence5.getFirstLetter() == 'a', 36);
		reportTest(sentence5.getMiddleLetter() == 'c', 37);
		reportTest(sentence5.getLastLetter() == 'd', 38);
		
		LimitedSentence sentence6 = new LimitedSentence('#');
		sentence6.appendWordEnd("boo");
		sentence6.appendWordEnd("hoo");
		reportTest(sentence6.getFirstLetter() == 'b', 39);
		reportTest(sentence6.getMiddleLetter() == '#', 40);
		reportTest(sentence6.getLastLetter() == 'o', 41);
		reportTest(sentence6.getIthWord(0).equals("boo"), 42);
		reportTest(sentence6.getIthWord(1).equals("hoo"), 43);
		reportTest(sentence6.getIthWord(7).equals(""), 44);
		
		System.out.println("Number of tests that failed: " + numFailed);
		System.out.println("Number of tests that passed: " + numSuccess);
		System.out.println("Done! Now you can add more tests!");
	}

	/**
	 * The method reports if the test failed or passed, according to the given condition.
	 * @param condition Condition to check. False value means that the test has failed.
	 * @param testNum Test number.
	 */
	private static void reportTest(boolean condition, int testNum) {
		if(! condition) {
			System.err.println("FAILED TEST " + testNum + ".");
			numFailed++;
		}
		else {
			System.out.println("PASSED TEST " + testNum + ".");
			numSuccess++;
		}
	}

}
