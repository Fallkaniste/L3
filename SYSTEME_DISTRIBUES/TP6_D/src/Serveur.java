import java.io.IOException;
import java.net.DatagramPacket;
import java.util.Scanner;

public class Serveur 
{

	public static void main(String[] args) 
	{
		int port=7777;
		
		if (args.length!=1)
		{
			System.out.println("Vous devez entrer uniquement le port.");
			System.exit(-1);
		}
		else
		{
			port=Integer.parseInt(args[0]);
		}
		
		try 
		{	
			String choice;
			int iChoice=-1;
			
			Scanner in = new Scanner(System.in);
			System.out.println("Reçevoir de manière :\n\t[0]Synchrone\n\t[1]Asynchrone\n");
	        choice = in.nextLine();
	        do
	        {
		        try
		        {
		        	iChoice=Integer.parseInt(choice);
		        }
		        catch (NumberFormatException e)
		        {
		        	System.out.println("Vous devez entrer un nombre.");
		        }
	        } while(iChoice==-1);

	        
	        
			AsynchronousDatagramSocket socket =new AsynchronousDatagramSocket(port);
			DatagramPacket dp = socket.synchronousReceive();
			System.out.println(dp.getData()[0]);
			socket.close();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}

}

