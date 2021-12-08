//Name: Ron Amar
//ID: 318479458
public abstract class Question 
{
	private String qText;
	
	public Question(String text) {
		this.qText = text;
	}
	
	public void setQText(String text) {
		this.qText = text;
	}
	
	public abstract String getCorrectAnswer();
	public abstract void addCorrectAnswer(String answer);
	
	public String toString() {
		return qText;
	}
}
