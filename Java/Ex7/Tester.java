public class Tester {
    private static final int MINUS_OF_FAILED_TEST = 2;
    private static int numFailed = 0;

    public static void main(String[] args) {
        int[] arr = {1, -3, 5, 6,  0, -4};
        String arrStr = ArrayUtils.toString(arr);
        reportTest(arrStr.equals("[1, -3, 5, 6, 0, -4]"), 1, "[1, -3, 5, 6, 0, -4]", arrStr,"toString");

        int[] abs = ArrayUtils.absArray(arr);
        String absStr = ArrayUtils.toString(abs);
        reportTest(absStr.equals("[1, 3, 5, 6, 0, 4]"), 2, "[1, 3, 5, 6, 0, 4]", absStr, "absArray");

        int[] arr2 = {1, -3, 5, 6,  0, -4};
        ArrayUtils.fill(arr2, 0);
        String arr2Str = ArrayUtils.toString(arr2);
        reportTest(arr2Str.equals("[0, 0, 0, 0, 0, 0]"), 3, "[0, 0, 0, 0, 0, 0]", arr2Str, "fill");

        int[] arr3 = {1, -3, 5, 6,  9, -4};
        ArrayUtils.fill(arr3, 0, 1, 4);
        String arr3Str = ArrayUtils.toString(arr3);
        reportTest(arr3Str.equals("[1, 0, 0, 0, 9, -4]"), 4, "[1, 0, 0, 0, 9, -4]" ,arr3Str, "fill");

        int[] arr4 = {1, 2, 3};
        int[] arr5 = {1, 2, 3, 4};
        reportTest(ArrayUtils.contains(arr4, arr5) == false, 5, false,ArrayUtils.contains(arr4, arr5) , "contains");
        reportTest(ArrayUtils.contains(arr5, arr4) == true, 6, true, ArrayUtils.contains(arr5, arr4), "contains");

        int[] arr6 = {1, -3, 5, 6,  9, -4};
        int[] arr6Reversed = ArrayUtils.getReverse(arr6);
        String arr6ReversedStr = ArrayUtils.toString(arr6Reversed);
        reportTest(arr6ReversedStr.equals("[-4, 9, 6, 5, -3, 1]"), 7, "[-4, 9, 6, 5, -3, 1]", arr6ReversedStr, "getReverse");

        int[] arr7 = {1, -3, 5, 6,  9, -4};
        ArrayUtils.reverse(arr7);
        String arr7Str = ArrayUtils.toString(arr7);
        reportTest(arr7Str.equals("[-4, 9, 6, 5, -3, 1]"), 8, "[-4, 9, 6, 5, -3, 1]", arr7Str, "reverse");

        int[] arr8 = {1, -3, 5, 6,  9, -4};
        int maxPos = ArrayUtils.getMaxPosition(arr8);
        reportTest(maxPos == 4, 9, 4, maxPos, "getMaxPosition");

        int[] arr9 = {1, 2, 3};
        int mult = ArrayUtils.getMult(arr9);
        reportTest(mult == 6, 10, 6, mult, "getMult");

        int[][] arr10 = {{1, 4, 2}, {16, 9 , 0}};
        int[] res = ArrayUtils.findMax(arr10);
        reportTest(ArrayUtils.toString(res).equals("[1, 0]") || ArrayUtils.toString(res).equals("[0, 1]"), 11, 1,res[0] , "findMax");

        int[][] arr11 = {{1, 4}, {16, 0}};
        int[][] arr12 = {{0, 6}, {4, 9}};
        int[][] resSum = ArrayUtils.sumTables(arr11, arr12);
        reportTest(resSum[0][0] == 1, 13, 1, resSum[0][0], "sumTables");
        reportTest(resSum[0][1] == 10, 14, 10,resSum[0][1], "sumTables");
        reportTest(resSum[1][0] == 20, 15, 20,resSum[1][0] , "sumTables");
        reportTest(resSum[1][1] == 9, 16,9, resSum[1][1] , "sumTables");

        //Input tests
        reportTest(ArrayUtils.toString(new int[0]).equals("[]"), 17, "[]",ArrayUtils.toString(new int[0]), "toString");
        reportTest(ArrayUtils.toString(null) == null, 18, null,ArrayUtils.toString(null), "toString");

        String nullCheck = ArrayUtils.toString(ArrayUtils.absArray(new int[0]));
        reportTest(nullCheck.equals("[]"), 19, "[]",nullCheck, "absArray");
        reportTest(ArrayUtils.absArray(null) == null, 20, null,ArrayUtils.absArray(null), "absArray");

        ArrayUtils.fill(new int[0], 0); //test 35
        ArrayUtils.fill(null, 2); //test 36

        ArrayUtils.fill(new int[0], 0, 5, 8); //test 37
        ArrayUtils.fill(null, 2, 5, 8); ////test 38
        ArrayUtils.fill(new int[0], 0, 3, 1); ////test 39
        int[] fillInputCheck = {3,5,7};
        ArrayUtils.fill(fillInputCheck, 2, 4, 4);
        reportTest(ArrayUtils.toString(fillInputCheck).equals("[3, 5, 7]"), 21, "[3, 5, 7]",ArrayUtils.toString(fillInputCheck), "fill");

        int[] containsInputCheck2 = {},containsInputCheck = {3,5,7};
        reportTest(!ArrayUtils.contains(containsInputCheck, containsInputCheck2), 22, false,ArrayUtils.contains(fillInputCheck, containsInputCheck2), "contains");

        reportTest(ArrayUtils.getMaxPosition(new int[0]) == -1, 23, -1,ArrayUtils.getMaxPosition(new int[0]), "getMaxPosition");
        reportTest(ArrayUtils.getMaxPosition(null) == -1, 24, -1,ArrayUtils.getMaxPosition(null), "getMaxPosition");

        reportTest(ArrayUtils.getMult(new int[0]) == Integer.MAX_VALUE, 25, Integer.MAX_VALUE,ArrayUtils.getMult(new int[0]), "getMult");
        reportTest(ArrayUtils.getMult(null) == Integer.MAX_VALUE, 26, Integer.MAX_VALUE,ArrayUtils.getMult(null), "getMult");

        int[][] findMaxInputCheck = {{-100,-30, -7}, {-4,-1,-2}};
        int[] findMaxInputCheck2 = {1,1};
        String result = ArrayUtils.toString(ArrayUtils.findMax(findMaxInputCheck));
        reportTest(result.equals(ArrayUtils.toString(findMaxInputCheck2)), 27, "[1, 1]",result, "findMax");
        reportTest(ArrayUtils.findMax(new int[0][0]) == null, 28, null, ArrayUtils.findMax(new int[0][0]), "findMax");
        reportTest(ArrayUtils.findMax(new int[0][3]) == null, 29, null,ArrayUtils.findMax(new int[0][3]), "findMax");
        reportTest(ArrayUtils.findMax(new int[6][0]) == null, 30, null,ArrayUtils.findMax(new int[6][0]), "findMax");
        reportTest(ArrayUtils.findMax(null) == null, 31, null, ArrayUtils.findMax(null), "findMax");

        int[][] sumTablesInputCheck = {{-1,2}, {3,5,-2}};
        int[][] sumTablesInputCheck2 = {{-1,2,6,7}, {3,5,-2}};
        reportTest(ArrayUtils.sumTables(null, sumTablesInputCheck) == null, 32, null, ArrayUtils.sumTables(null, sumTablesInputCheck), "sumTables");
        reportTest(ArrayUtils.sumTables(sumTablesInputCheck, null) == null, 33, null, ArrayUtils.sumTables(sumTablesInputCheck, null), "sumTables");
        reportTest(ArrayUtils.sumTables(sumTablesInputCheck, sumTablesInputCheck2) == null, 34, null, ArrayUtils.sumTables(sumTablesInputCheck, sumTablesInputCheck2), "sumTables");

        System.out.println("~~YOUR SCORE: " + (100-(MINUS_OF_FAILED_TEST *numFailed)) );

    }


    /**
     * The method reports if the test failed or passed, according to the given condition.
     * @param condition Condition to check. False value means that the test has failed.
     * @param testNum Test number.
     */
    private static void reportTest(boolean condition, int testNum, Object expected_res, Object your_res, String funcName) {
        if(!condition){
            System.err.println("FAILED TEST " + testNum + ". At function: " +funcName + ". expected result: " + expected_res + " You're result:  " + your_res);
            numFailed++;
        }
    }

}
