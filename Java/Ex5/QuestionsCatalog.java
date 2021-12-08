//Name: Ron Amar
//ID: 318479458
public class QuestionsCatalog 
{
	private Question[] questions;
	private final int MAX_QUESTIONS = 20;
	private int numberOfQuestions;
	public static final int SHORT = 1, MULTIPLE = 2, BOTH = 3;
	
	public QuestionsCatalog() {
		this.questions = new Question[MAX_QUESTIONS];
		this.numberOfQuestions = 0;
	}
	
	private boolean isQFull() {
		if(numberOfQuestions == MAX_QUESTIONS) return true;
		return false;
	}
	
	public void addQuestion(Question q) {
		if(isQFull()) return;
		questions[numberOfQuestions] = q;
		numberOfQuestions++;
	}
	
	//Generates an array with random integer values that are non-repetitive from 0 to a given number (random indexes)
	private int[] generateOrder() {
		int[] indexes = new int[numberOfQuestions];
		indexes[0] = (int)(Math.random() * numberOfQuestions);
		int i = 1;
		
		while(i < indexes.length) {
			boolean flag = true;
			int num = (int)(Math.random() * numberOfQuestions);
			
			for(int j=0; j<i; j++) 
				if(indexes[j] == num) {
					flag = false;
					break;
				}
			
			if(flag) {
				indexes[i] = num;
				i++;
			}
		}
		return indexes;
	}
	
	//Checks how many questions stored in the array are of a specific type
	private int howManyOfType(int type) {
		if(type == 3) return numberOfQuestions;
		int counter = 0;
		for(int i=0; i<numberOfQuestions; i++) {
			switch(type) {
				case 1:
					if(questions[i] instanceof ShortAnswerQuestion)
						counter++;
					break;
				case 2:
					if(questions[i] instanceof MultipleChoiceQuestion)
						counter++;
			}
		}
		return counter;
	}
	
	private int checkType(int type) {
		if(type > 3 || type <1) type =3;
		return type;
	}

	public Question[] generateQuestionnaire(int num, int type) {
		if(num < 1) return null;
		//makes sure 'type' is between 1-3. if not sets it to 3.
		type = checkType(type);
		//checks how many questions there are of a specific type
		int numOfType = howManyOfType(type);
		if(numOfType == 0) return null;
		//making sure we don't ask for more questions than we can give
		if (num > numOfType) num = numOfType;
		//generates an array of random non-repetitive indexes to be used later
		int[] index = generateOrder();
		Question[] toReturn = new Question[num];
		
		int i = 0;
		int j = 0;
		
		while(i<num) {
			boolean flag = false;
			for(; j<numberOfQuestions; j++) {
				switch(type) {
				case 1:
					if(questions[index[j]] instanceof ShortAnswerQuestion) {
						toReturn[i] = questions[index[j]];
						flag = true;
					}
					break;
					
				case 2:
					if(questions[index[j]] instanceof MultipleChoiceQuestion) {
						toReturn[i] = questions[index[j]];
						flag = true;
					}
					break;
				
				case 3:
					toReturn[i] = questions[index[j]];
					flag=true;
				}
				if(flag) {
					i++;
					j++;
					break;
				}
			}
		}
		
		return toReturn;
	}
}
