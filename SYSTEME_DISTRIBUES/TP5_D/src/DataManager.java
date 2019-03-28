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
	
	public Personne getPersonne(int id) throws InvalidIdException
	{
		try
		{
			Personne p=list.get(id);
			return p;
		}
		catch(IndexOutOfBoundsException e)
		{
			throw new InvalidIdException();
		}
	}
	
	public int getID(Personne p)
	{
		return list.indexOf(p);
	}
	
}
