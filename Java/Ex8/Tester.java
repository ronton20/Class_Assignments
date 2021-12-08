public class Tester {
	private static int numFailed = 0;
	private static int numSuccess = 0;
	private static int total = 0;


	public static void main(String[] args) {
		// test isDna:
		reportTest(DnaSequence.isDnaSequence("acgt"), 1);
		reportTest(!DnaSequence.isDnaSequence("hello hello"), 2);
		
		DnaSequence gene1 = new DnaSequence("ACGtTgAaT");
		DnaSequence gene2 = new DnaSequence("ACGTHELLO");
		DnaSequence gene3 = new DnaSequence("ACGT");
		
		// test initialization & toString():
		reportTest(gene3.toString().equals("ACGT: (1, 1, 1, 1)"), 3);
		reportTest(gene2.toString().equals("ACGT: (1, 1, 1, 1)"), 4);
		reportTest(gene1.toString().equals("ACGtTgAaT: (3, 1, 2, 3)"), 5);
		
		// test copy constructor:
		DnaSequence gene4 = new DnaSequence(gene3);
		reportTest(areEqualArrays(gene3.getNucleotideCounts(), gene4.getNucleotideCounts()), 6);
		
		// test changeCapitalization():
		gene1.changeCapitalization();
		reportTest(gene1.toString().equals("acgTtGaAt: (3, 1, 2, 3)"), 7);
		
		gene2.changeCapitalization();
		reportTest(gene2.toString().equals("acgt: (1, 1, 1, 1)"), 8);
		
		// test get & set:
		int[] arr = {1, 1, 1, 1};
		reportTest(areEqualArrays(gene3.getNucleotideCounts(), arr), 9);
		reportTest(gene1.getSequence().equals("acgTtGaAt"), 10);
		gene3.setSequence("tgagtGGGGca");
		reportTest(gene3.getSequence().equals("tgagtGGGGca"), 11);
		int[] newCounts = {2, 1, 6, 2};
		reportTest(areEqualArrays(gene3.getNucleotideCounts(), newCounts), 12);

				
		// test addArray:
		int[] arr1 = {1, 2, -3, 4};
		int[] arr2 = {0, -2, 4, 6};
		int[] resSum = {1, 0, 1, 10};
		DnaSequence.addArray(arr1, arr2);
		reportTest(areEqualArrays(arr1, resSum), 13);
		
		
		// test concatGenes:
		gene1.concatGenes(gene2);
		int[] res = {4, 2, 3, 4};
		reportTest(areEqualArrays(gene1.getNucleotideCounts(), res), 14);
		reportTest(gene1.getSequence().equals("acgTtGaAtacgt"), 15);


		System.out.println("Num. test failed: " +  numFailed);
		System.out.println("Num. test passed: " + numSuccess);
		System.out.println("Num. total tests: " + total);
		System.out.println("Original tester is done! Now you can add more tests.");
	}

	
	/**
	 * The method reports if the test failed or passed, according to the given condition.
	 * @param condition Condition to check. False value means that the test has failed.
	 * @param testNum Test number.
	 */
	private static void reportTest(boolean condition, int testNum) {
		total++;
		if(! condition) {
			System.err.println("FAILED TEST " + testNum + ".");
			numFailed++;
		}
		else {
			System.out.println("PASSED TEST " + testNum + ".");
			numSuccess++;
		}
	}
	
	private static boolean areEqualArrays(int[] arr1, int[] arr2) {
		if(arr1 == null || arr2 == null)
			return false;
		
		if(arr1.length != arr2.length)
			return false;
		
		for (int i = 0; i < arr2.length; i++) {
			if(arr1[i] != arr2[i])
				return false;
		}
		
		return true;
	}
}
