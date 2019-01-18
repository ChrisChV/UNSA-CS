import HJ.Hilo;

public class Main{
	static int numHilos = 10;
	public static void main(String[] args){
		Hilo[] hilos;
		hilos = new Hilo[numHilos];
		for(int i = 0; i < numHilos; i++){
			hilos[i] = new Hilo(i);
		}
		for(int i = 0; i < numHilos; i++){
			hilos[i].start();
		}
	}
}