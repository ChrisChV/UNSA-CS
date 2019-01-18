package HJ;
import java.lang.*;


public class Hilo extends Thread{
	private int number;

	public Hilo(int num){
		number = num;
	}

	@Override
	public void run()	{
		System.out.println("Holaaaa soy el hilo " + number);
	}
}

