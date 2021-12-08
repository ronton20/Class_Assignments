import javax.swing.JFrame;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(frame.EXIT_ON_CLOSE);
		frame.setSize(500, 500);
		
		CatchMe panel = new CatchMe();
		frame.add(panel);
		
		frame.setVisible(true);
	}

}
