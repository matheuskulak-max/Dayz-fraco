////////////////////////////////////////////////////
class VVN_Greenhouse_Kit extends KitBase
{    
    override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
    {
        super.OnPlacementComplete( player, position, orientation );
        
        if ( GetGame().IsServer() )
        {
            
            EntityAI greenhouse = EntityAI.Cast( GetGame().CreateObjectEx( "VVN_greenhouse", GetPosition(), ECE_PLACE_ON_SURFACE ) );
            greenhouse.SetPosition( position );
            greenhouse.SetOrientation( orientation );
        }    
    }
    override void UpdateVisuals()
	{
        return;
	}
	
	override void UpdatePhysics()
	{
        return;
	}
};
/////////////////////////////////////////////////////

