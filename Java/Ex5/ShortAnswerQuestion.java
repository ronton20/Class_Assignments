//Name: Ron Amar
//ID: 318479458
public class ShortAnswerQuestion extends Question 
{
	private String answer;
	
	public ShortAnswerQuestion(String question, String answer) {
		super(question);
		this.answer = answer;
	}
	
	@Override
	public String getCorrectAnswer() {
		return this.answer;
	}

	@Override
	public void addCorrectAnswer(String answer) {
		this.answer = answer;
	}
	
}
