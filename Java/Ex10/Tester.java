import javax.swing.*;

public class Tester {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		frame.setSize(1000, 1000);
		FlagsPanel panel = new FlagsPanel(10, 10);
		frame.add(panel);
		frame.setVisible(true);
	}

}
