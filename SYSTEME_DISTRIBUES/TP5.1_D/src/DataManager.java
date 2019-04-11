import java.util.ArrayList;

public class DataManager 
{
	private ArrayList<Personne> list;
	
	public DataManager()
	{
		this.list=new ArrayList<>();
	}
	
	public int addPersonne(Personne p)
	{
		if(!list.contains(p))
		{
			list.add(p);
		}	

		return getID(p);
		
	}
	
	public Personne getPersonne(Integer id)
	{
		Personne p;
		try
		{
			p=list.get(id);
			
		}
		catch (IndexOutOfBoundsException e)
		{
			p=null;
		}
		return p;
	}
	
	public int getID(Personne p)
	{
		return list.indexOf(p);
	}
	
}
