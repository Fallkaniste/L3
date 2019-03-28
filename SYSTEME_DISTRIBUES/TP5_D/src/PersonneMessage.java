
public class PersonneMessage extends Message 
{
	private static final long serialVersionUID = 1550243136891998385L;
	private Personne p;
	
	public PersonneMessage()
	{
		super();
	}
	
	public PersonneMessage(Personne p)
	{
		super();
		this.p=p;
	}
	
	public void setPersonne(Personne p)
	{
		this.p=p;
	}
	
	public Personne getPersonne()
	{
		return this.p;
	}

}
