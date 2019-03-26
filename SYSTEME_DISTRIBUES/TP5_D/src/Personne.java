import java.io.Serializable;

public class Personne implements Serializable
{
	private static final long serialVersionUID = 1L;
	private String name;
	
	public Personne()
	{
		this.name="defaultName";
	}
	
	public Personne(String name)
	{
		this.name=name;
	}
	
	@Override
	public String toString()
	{
		return this.name;
	}
}
