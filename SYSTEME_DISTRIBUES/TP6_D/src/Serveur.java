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
	        do
	        {
	        	if(iChoice!=-1)
	        	{
	        		System.out.println("Saisie incorrecte. " + iChoice + " n'est pas un nombre valide.");
	        		iChoice=-1;
	        	}
	        	
		        choice = in.nextLine();

		        try
		        {
		        	iChoice=Integer.parseInt(choice);
		        }
		        catch (NumberFormatException e)
		        {
		        	System.out.println("Saisie incorrecte. "+ choice + " n'est pas un nombre.");
		        }
	        } while(!(iChoice==0 || iChoice==1));
        
			AsynchronousDatagramSocket socket =new AsynchronousDatagramSocket(port);
			DatagramPacket dp;
			
			if(iChoice==0)
			{
				dp= socket.synchronousReceive();
				System.out.println("Reçu :"+(char)dp.getData()[0]);
			}
			else if(iChoice==1)
			{
				do
				{
					dp = socket.asynchronousReceive();
					if(dp==null)
					{
						System.out.println("Aucun paquet reçu");
					}
					else
					{
						System.out.println("Reçu :"+(char)dp.getData()[0]);
					}
					
					iChoice=-1;
					System.out.println("Voulez vous tenter une nouvelle réception?\n\t[0]Oui\n\t[1]Non\n");
			        do
			        {
			        	if(iChoice!=-1)
			        	{
			        		System.out.println("Saisie incorrecte. " + iChoice + " n'est pas un nombre valide.");
			        		iChoice=-1;
			        	}
			        	
				        choice = in.nextLine();

				        try
				        {
				        	iChoice=Integer.parseInt(choice);
				        }
				        catch (NumberFormatException e)
				        {
				        	System.out.println("Saisie incorrecte. "+ choice + " n'est pas un nombre.");
				        }
			        } while(!(iChoice==0 || iChoice==1));
			
				} while(iChoice==0);
				
			}
			
	        in.close();
	        
			socket.close();
			
			
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}

}

