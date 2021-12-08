//Name: Ron Amar
//ID: 318479458
public class MultipleChoiceQuestion extends Question {
	
	private String[] answers;
	private int numberOfAnswers;
	private int correctAnswerIndex;
	private final int MAX_ANSWERS = 6;
	
	public MultipleChoiceQuestion(String question, String answer) {
		super(question);
		this.answers = new String[MAX_ANSWERS];
		this.answers[0] = answer;
		numberOfAnswers = 1;
		correctAnswerIndex = 0;
	}
	
	private boolean isAnsFull() {
		if (numberOfAnswers >= MAX_ANSWERS) return true;
		return false;
	}
	
	@Override
	public String getCorrectAnswer() {
		return (correctAnswerIndex + 1) + "";
	}

	@Override
	public void addCorrectAnswer(String answer) {
		if(isAnsFull()) return;
		answers[numberOfAnswers] = answer;
		correctAnswerIndex = numberOfAnswers;
		numberOfAnswers++;

	}
	
	public void addAnswer(String answer) {
		if(isAnsFull()) return;
		answers[numberOfAnswers] = answer;
		numberOfAnswers++;
	}
	
	public String toString() {
		String ans = super.toString() + "\n";
		for(int i = 0; i<numberOfAnswers; i++) {
			ans += (i + 1) + ") " + answers[i] + "\n";
		}
		return ans;
	}
}
