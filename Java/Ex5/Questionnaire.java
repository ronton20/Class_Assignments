//Name: Ron Amar
//ID: 318479458
public class Questionnaire 
{
	private Question[] cat;
	private int totalQuestions;
	private int currentQuestion;
	private int correctAnswers;
	
	public Questionnaire(int type, int num, QuestionsCatalog cat) {
		this.cat = cat.generateQuestionnaire(num, type);
		this.totalQuestions = this.cat.length;
		this.correctAnswers = 0;
		this.currentQuestion = 0;
	}
	
	public boolean hasNext() {
		if(currentQuestion < totalQuestions) return true;
		return false;
	}
	
	public Question getNext() {
		if(currentQuestion == totalQuestions) return null;
		return cat[currentQuestion];
	}
	
	public void checkAnswer(String answer) {
		if(cat[currentQuestion].getCorrectAnswer().equals(answer)) {
			correctAnswers++;
		}
		currentQuestion++;
	}
	
	public int getTotalQuestions() {
		return totalQuestions;
	}
	
	public int getCorrectAnswers() {
		return correctAnswers;
	}
}
