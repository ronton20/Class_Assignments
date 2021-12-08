// Ron Amar
// 318479458

public class ArrayUtils 
{
	private static boolean isArrValid(int[] arr)	//checks if an array is valid
	{
		if(arr==null)
			return false;
		return true;
	}
	
	private static boolean isArrEmpty(int[] arr)
	{
		if(arr.length==0)
			return true;
		return false;
	}
	
	/**
	 * prints a given array
	 * @param arr entered array
	 * @return String of the array
	 */
	public static String toString(int[] arr)
	{
		if(!isArrValid(arr))
			return null;
		if(isArrEmpty(arr))
			return "[]";
		String ans="[";
		for(int i=0; i<arr.length; i++)
		{
			ans+=arr[i];
			if(i<arr.length-1)
				ans+=", ";
		}
		ans+="]";
		return ans;
	}
	
	/**
	 * setting all the values of the array to positive numbers
	 * @param arr given array
	 * @return a new array with only positive values of the same numbers
	 */
	public static int[] absArray(int[] arr)
	{
		if(!isArrValid(arr))
			return null;
		if(isArrEmpty(arr))
			return new int[0];
		int[] newArr=new int[arr.length];
		for(int i=0; i<newArr.length; i++)
			newArr[i]=Math.abs(arr[i]);
		return newArr;
	}
	
	/**
	 * fills a complete array with a given number
	 * @param arr array to fill
	 * @param value number to fill the array with
	 */
	public static void fill(int[] arr, int value)
	{
		if(!isArrValid(arr))
			return;
		if(isArrEmpty(arr))
			return;
		for(int i=0; i<arr.length; i++)
			arr[i]=value;
	}
	
	/**
	 * replaces the values of a given array from a starting index to an ending index with a given number
	 * @param arr array to change
	 * @param value number to fill the array with
	 * @param start index of starting point
	 * @param end index of ending point
	 */
	public static void fill(int[] arr, int value, int start, int end)
	{
		if(!isArrValid(arr))
			return;
		if(isArrEmpty(arr))
			return;
		if(start<0 || end>arr.length)
			return;
		for(int i=0; i<arr.length; i++)
			if(i>=start && i<end)
				arr[i]=value;
	}
	
	/**
	 * checks if an array is containing the values of a second array
	 * @param arr1 the array containing
	 * @param arr2 the array contained
	 * @return if arr2's values are found inside arr1
	 */
	public static boolean contains(int[] arr1, int[] arr2)
	{
		if(!isArrValid(arr1) || !isArrValid(arr2))
			return false;
		if(isArrEmpty(arr1) || isArrEmpty(arr2))
			return false;
		boolean flag=true;
		for(int i=0; i<arr2.length; i++)
		{
			if(flag)
			{
				for(int j=0; j<arr1.length; j++)
				{
					if(arr2[i]==arr1[j])
						{
						flag=true;
						break;
						}
					else
						flag=false;
				}
			}
			else
				break;
		}
		return flag;
	}
	
	/**
	 * gives the index of the highest number in the array
	 * @param arr array to check
	 * @return the index of the highest number in the array
	 */
	public static int getMaxPosition(int[] arr)
	{
		if(!isArrValid(arr) || isArrEmpty(arr))
			return -1;
		int index=0;
		for(int i=1; i<arr.length; i++)
		{
			if(arr[i]>arr[index])
				index=i;
		}
		return index;
	}
	
	/**
	 * mutiplies the values of a given array
	 * @param arr the array
	 * @return a number that is the multiplication of all the values in the array
	 */
	public static int getMult(int[] arr)
	{
		if(!isArrValid(arr) || isArrEmpty(arr))
			return Integer.MAX_VALUE;
		int sum=1;
		for(int i=0; i<arr.length; i++)
			sum*=arr[i];
		return sum;
	}
	
	/**
	 * creates a new array the is the same as a given array but reversed
	 * @param arr the given array
	 * @return a new reversed array
	 */
	public static int[] getReverse(int[] arr)
	{
		if(arr==null)
			return null;
		if(isArrEmpty(arr))
			return new int[0];
		int[] newArr=new int[arr.length];
		for(int i=0; i<arr.length; i++)
			newArr[i]=arr[arr.length-1-i];
		return newArr;
	}
	
	/**
	 * reverses the order of parameters of a given array
	 * @param arr array to reverse
	 */
	public static void reverse(int[] arr)
	{
		if(!isArrValid(arr))
			return;
		if(isArrEmpty(arr))
			return;
		for(int i=0; i<arr.length/2; i++)
		{
			int temp = arr[i];
			arr[i]=arr[arr.length-1-i];
			arr[arr.length-1-i]=temp;
		}
	}
	
	/**
	 * finds the index of a 2 dimentional array and puts its index in a new array
	 * @param arr table to search
	 * @return a new array with the values of the indexes of the max number in the table
	 */
	public static int[] findMax(int[][] arr)
	{
		if(arr==null || arr.length==0)
			return null;
		int[] ans=new int[]{0,0};
		for(int i=0; i<arr.length; i++)
		{
			if(!isArrValid(arr[i]) || isArrEmpty(arr[i]))
				return null;
			for(int j=0; j<arr[i].length; j++)
			{
				if(arr[i][j]>arr[ans[0]][ans[1]])
				{
					ans[0]=i;
					ans[1]=j;
				}
			}
		}
		return ans;
	}
	
	/**
	 * combines 2 tables only if they have the same dimentions
	 * @param table1 1st table
	 * @param table2 2nd table
	 * @return a new table with the added values from each table in the same spot
	 */
	public static int[][] sumTables(int[][] table1, int[][] table2)
	{
		if(table1==null || table2==null)
			return null;
		if(table1.length!=table2.length)
			return null;
		for(int i=0; i<table1.length; i++)
			if(table1[i]==null || table2[i]==null)
				return null;
		for(int i=0; i<table1.length; i++)
			if(table1[i].length!=table2[i].length)
				return null;
		
		int[][] sum=new int[table1.length][];
		for(int i=0; i<sum.length; i++)
		{
			int[] temp=new int[table1[i].length];
			for(int j=0; j<table1.length; j++)
			{
				temp[j]=table1[i][j]+table2[i][j];
			}
			sum[i]=temp;
		}
		return sum;
	}
}
