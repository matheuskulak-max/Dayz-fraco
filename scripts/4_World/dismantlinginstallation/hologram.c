modded class Hologram
{
	override string ProjectionBasedOnParent()
	{
		VVN_Greenhouse_Kit cabin_in_hands = VVN_Greenhouse_Kit.Cast( m_Player.GetHumanInventory().GetEntityInHands() );
		if ( cabin_in_hands )
		{
			return "VVN_Greenhouse_KitPlacing";
		}
		return super.ProjectionBasedOnParent();
	};

	vector ItemPlacementOffset()
	{
		return "1 0 0";
	}

	override void UpdateHologram(float timeslice)
	{
		super.UpdateHologram(timeslice);
		ItemBase cabinItem = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());
		if (cabinItem && cabinItem.IsKindOf("VVN_Greenhouse_Kit"))
		{
			vector containerPos = GetProjectionEntityPosition( m_Player ) + ItemPlacementOffset();
			// Убираем AlignProjectionOnTerrain() и оставляем текущую ориентацию без изменений
			vector containerOrientation = m_Projection.GetOrientation(); // Сохраняем текущую ориентацию
			SetProjectionPosition( containerPos );
			SetProjectionOrientation( containerOrientation );
			m_Projection.OnHologramBeingPlaced( m_Player );
			return;
		}
	}

	override EntityAI PlaceEntity( EntityAI entity_for_placing )
	{
		ItemBase item_in_hands = ItemBase.Cast( m_Player.GetHumanInventory().GetEntityInHands() );

		if ( m_Projection.IsInherited( GardenPlotPlacing ) ) //garden plot
		{
			Class.CastTo(entity_for_placing, GetGame().CreateObjectEx( "GardenPlot", m_Projection.GetPosition(), ECE_OBJECT_SWAP )); //garden plot
			return entity_for_placing;
		}

		if( entity_for_placing.CanAffectPathgraph() )
		{
			entity_for_placing.SetAffectPathgraph( true, false );
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, entity_for_placing);
		}
		return entity_for_placing;
	}

	override void EvaluateCollision(ItemBase action_item = null)
	{
		if (m_Parent && m_Parent.IsInherited(VVN_Greenhouse_Kit))
		{
			SetIsColliding(false);
			return;
		}
		super.EvaluateCollision();
	}

	override bool IsFloating()
	{
		if (m_Parent && m_Parent.IsInherited(VVN_Greenhouse_Kit))
		{
			return true;
		}
		return super.IsFloating();
	}

	override void SetProjectionPosition(vector position)
	{
		if (m_Parent && m_Parent.IsInherited(VVN_Greenhouse_Kit) && IsFloating())
		{
			vector itemPos = SetOnGroundOld( position );
			m_Projection.SetPosition( itemPos );
			return;
		}
		super.SetProjectionPosition(position);
	}

	vector SetOnGroundOld( vector position )
	{
		vector from = position;
		vector ground;
		vector player_to_projection_vector;
		float projection_diameter = GetProjectionDiameter();

		ground = Vector(0, - Math.Max( projection_diameter, SMALL_PROJECTION_GROUND ), 0);
		vector to = from + ground;
		vector contact_pos;
		int contact_component;

		DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, contact_component, NULL, NULL, m_Projection, false, false );

		HideWhenClose( contact_pos );

		return contact_pos;
	}
};