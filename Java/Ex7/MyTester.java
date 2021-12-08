
//public class MyTester {
//
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//		int[] arr1={1,-2,3,-4,-5,6,7,8};
//		int[] arr2= {};
//
//		System.out.println(ArrayUtils.toString(arr1));
//		System.out.println(ArrayUtils.toString(arr2));
//		System.out.println("Contains? " + ArrayUtils.contains(arr1, arr2));
//		System.out.println("---------------------");
//
//		int[] arr3=ArrayUtils.absArray(arr1);
//		System.out.println(ArrayUtils.toString(arr1));
//		System.out.println(ArrayUtils.toString(arr3));
//		System.out.println("---------------------");
//
//		ArrayUtils.fill(arr2, 5);
//		ArrayUtils.fill(arr3, 5, 2, 4);
////		arr3=new int[0];
//
//		System.out.println(ArrayUtils.toString(arr3));
//		System.out.println(ArrayUtils.toString(arr2));
//		System.out.println("Contains? " + ArrayUtils.contains(arr3, arr2));
//		System.out.println("---------------------");
//
//		int[] revArr3=ArrayUtils.getReverse(arr3);
//		System.out.println(ArrayUtils.toString(revArr3));
//		ArrayUtils.reverse(arr3);
//		System.out.println(ArrayUtils.toString(arr3));
//		System.out.println("---------------------");
//
//		int [][] table1={{1,2,3},{10,4,17},{6,11,8}};
//		int[] max=ArrayUtils.findMax(table1);
//		for(int i=0; i<table1.length; i++)
//		{
//			System.out.println(ArrayUtils.toString(table1[i]));
//		}
//		System.out.println("Max index: " + ArrayUtils.toString(max));
//		System.out.println("---------------------");
//
//		int [][] table2= {{1,2,3},{10,4,17},{6,11,8}};
//		int[][] sum=ArrayUtils.sumTables(table1, table2);
//		if(sum!=null)
//		{
//			for(int i=0; i<table1.length; i++)
//			{
//				System.out.println(ArrayUtils.toString(sum[i]));
//			}
//		}
//	}
//}
import java.util.Arrays;
public class MyTester {
	private static int numFailed = 0;
    private static int numSuccess = 0;

    public static void main(String[] args) {
        int[] arr = {1, -3, 5, 6,  0, -4};
        int[] arrn =null;
        int[] arrn2 =null;
        int[] arr0 = {};
        int[] arr02 = {};
        int[] temp = new int[arr.length];


        String arr0Str = ArrayUtils.toString(arr0);
        reportTest (arr0Str.equals("[]"),1);
        String arrrStr = ArrayUtils.toString(arrn);
        String arrStr = ArrayUtils.toString(arr);
        reportTest (arrrStr==null,1);
        reportTest(arrStr.equals("[1, -3, 5, 6, 0, -4]"), 1);

        int[] abs = ArrayUtils.absArray(arr);
        int[] absn= ArrayUtils.absArray(arrn);

        arrStr = ArrayUtils.toString(arr);
        String absStr = ArrayUtils.toString(abs);
        String absStrn = ArrayUtils.toString(absn);

        reportTest(arrStr.equals("[1, -3, 5, 6, 0, -4]"), 2);
        reportTest(absStr.equals("[1, 3, 5, 6, 0, 4]"), 3);
        reportTest(absStrn==null, 3);


        int[] arr2 = {1, -3, 5, 6,  0, -4};
        ArrayUtils.fill(arr2, 0);
        ArrayUtils.fill(arrn, 0);

        String arr2Str = ArrayUtils.toString(arr2);
        String arrnStr = ArrayUtils.toString(arrn);

        reportTest(arr2Str.equals("[0, 0, 0, 0, 0, 0]"), 4);
        reportTest(arrnStr==null, 4);

        int[] arr3 = {1, -3, 5, 6,  9, -4};
        ArrayUtils.fill(arr3, 0, 1, 4);
        String arr3Str = ArrayUtils.toString(arr3);
        reportTest(arr3Str.equals("[1, 0, 0, 0, 9, -4]"), 5);

        int[] arr4 = {1, 2, 3};
        int[] arr5 = {1, 2, 3, 4};
        int[] arr000 = {0,0,0};
        int[] arr0000 = {0,0,0,0};

        reportTest(ArrayUtils.contains(arr4, arr5) == false, 6);
        reportTest(ArrayUtils.contains(arr5, arr4) == true, 7);
        reportTest(ArrayUtils.contains(arr0, arr02) == false, 7);//checks if null and null is false
        reportTest(ArrayUtils.contains(arrn, arrn2) == false, 7);// both ways
        reportTest(ArrayUtils.contains(arr0000, arr000) == true, 7);//checks if a {0,0,0} is contained in {0,0,0,0}
        reportTest(ArrayUtils.contains(arr000, arr0000) == true, 7);//checks if a {0,0,0,0} is contained in {0,0,0}
        reportTest(ArrayUtils.contains(arr4, arr02) == false, 7);//checks if a {0,0,0,0} is contained in {0,0,0}

        int[] arr6 = {1, -3, 5, 6,  9, -4};
        int[] arr6Reversed = ArrayUtils.getReverse(arr6);
        String arr6ReversedStr = ArrayUtils.toString(arr6Reversed);
        reportTest(arr6ReversedStr.equals("[-4, 9, 6, 5, -3, 1]"), 8);

        int[] arr7 = {1, -3, 5, 6,  9, -4};
        System.out.println (ArrayUtils.toString(arr7));
        ArrayUtils.reverse(arr7);
        String arr7Str = ArrayUtils.toString(arr7);
        System.out.println (arr7Str);;
        reportTest(arr7Str.equals("[-4, 9, 6, 5, -3, 1]"), 9);

        int[] arr8 = {1, -3, 5, 6,  9, -4};
        int[] arrNegatives = {-2, -3, -5, -6, -9, -4};

        int maxPos = ArrayUtils.getMaxPosition(arr8);
        int maxposN = ArrayUtils.getMaxPosition(arrn);
        int maxPosOfNigative = ArrayUtils.getMaxPosition(arrNegatives);
        reportTest(maxPos == 4, 10);
        reportTest(maxposN == -1, 10);//checks if maxPos of Null is -1
        reportTest(maxposN == -1, 10);//checks if maxPos of Null is -1
        reportTest(maxPosOfNigative == 0, 10);//checks if maxPos of Null is -1



        int[] arr9 = {1, 2, 3};
        int mult = ArrayUtils.getMult(arr9);
        reportTest(mult == 6, 11);
        int[][] arrDouble = {{ -1, -2, -4},{-3, -5, -7}};
        int[] resDouble = ArrayUtils.findMax (arrDouble);
        int[][] arr10 = {{1, 4, 2}, {16, 9 , 0}};
        int[] res = ArrayUtils.findMax(arr10);
        int[][] arrnS = null;
        int[] resN = ArrayUtils.findMax(arrnS);
        reportTest(resN==null, 12);//checks if findmax of Null is null
        reportTest(res[0] == 1, 12);
        reportTest(res[1] == 0, 13);
        reportTest(res[1] == 0, 13);
        reportTest(resDouble[0] == 0, 13);
        reportTest(resDouble[1] == 0, 13);



        int[][] arr11 = {{1, 4},
                {16, 0}};
        int[][] arr12 = {{0, 6},
                {4, 9}};
        int[][] tempDoubleArr12 = new int[arr12.length][arr12[0].length];
        int[][] tempDoubleArr11 = new int[arr12.length][arr12[0].length];

        for(int i=0;i< arr12.length;i++){
            for(int j=0;j< arr12.length;j++){
                tempDoubleArr12[i][j] = arr12[i][j];
            }
        }
        for(int i=0;i< arr11.length;i++){
            for(int j=0;j< arr11.length;j++){
                tempDoubleArr11[i][j] = arr11[i][j];
            }
        }
        int[][] resSum = ArrayUtils.sumTables(arr11, arr12);
        reportTest(resSum[0][0] == 1, 14);
        reportTest(resSum[0][1] == 10, 15);
        reportTest(resSum[1][0] == 20, 16);
        reportTest(resSum[1][1] == 9, 17);
        reportTest(Arrays.deepEquals (arr12, tempDoubleArr12), 17);
        reportTest(Arrays.deepEquals (arr11, tempDoubleArr11), 17);



        System.out.println("num. test failed: " +  numFailed);
        System.out.println("num. test passed: " + numSuccess);

        System.out.println("Done!");
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
