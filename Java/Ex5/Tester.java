import java.util.Scanner;
public class Tester 
{

	public static void main(String[] args) 
	{
		
		Scanner in = new Scanner(System.in);
		
		QuestionsCatalog cat = new QuestionsCatalog();
		Question q1 = new ShortAnswerQuestion("How many legs does a spider have?", "8");
		cat.addQuestion(q1);
		
		MultipleChoiceQuestion q2 = new MultipleChoiceQuestion("Where does the sun rises?", "East");
		q2.addAnswer("North");
		q2.addAnswer("South");
		q2.addAnswer("West");
		cat.addQuestion(q2);
		
		MultipleChoiceQuestion q3 = new MultipleChoiceQuestion("How many circles are in the Olympics's symbol?", "5");
		q3.addAnswer("4");
		q3.addAnswer("6");
		q3.addAnswer("8");
		cat.addQuestion(q3);
		
		MultipleChoiceQuestion q4 = new MultipleChoiceQuestion("How many triangles are in a pyramid?", "4");
		q4.addAnswer("5");
		q4.addAnswer("3");
		q4.addAnswer("6");
		cat.addQuestion(q4);
		
		Question q5 = new ShortAnswerQuestion("How many circles are in Audi's symbol", "4");
		cat.addQuestion(q5);
		
		Question q6 = new ShortAnswerQuestion("What is the color of water?", "Transparent");
		cat.addQuestion(q6);
		
		Question q7 = new ShortAnswerQuestion("How many lanes are there in League of Legends?", "3");
		cat.addQuestion(q7);
		
		Questionnaire qnr = new Questionnaire(QuestionsCatalog.BOTH, 7, cat);
		
		System.out.println("Welcome to our questionnaire, it starts now!");
		
		while(qnr.hasNext()) {
			Question q = qnr.getNext();
			System.out.println(q);
			System.out.println("Your answer:");
			String answer = in.nextLine();
			qnr.checkAnswer(answer);
		}
		
		System.out.println("Thank you for participating in our test.");
		int correct = qnr.getCorrectAnswers();
		int total = qnr.getTotalQuestions();
		System.out.println("You've answered " + correct + " correct answers out of " + total + " questions.");
	}

}
