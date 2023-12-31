	//Not finished there might be missing nodes. LMK!
        inline const std::unordered_map<std::string, uint64_t> DataNodeOffsets {
		{"CVehicleSyncTree_CVehicleCreationDataNode", 0x1680},
		{"CVehicleSyncTree_CVehicleProximityMigrationDataNode", 0x1760},
		{"CVehicleSyncTree_CVehicleSteeringDataNode", 0x19E8},
		{"CVehicleSyncTree_CVehicleControlDataNode", 0x1AB0},
		{"CVehicleSyncTree_CVehicleDamageStatusDataNode", 0x1E50},
		{"CVehicleSyncTree_CVehicleGameStateDataNode", 0x1F80},
		{"CVehicleSyncTree_CVehicleHealthDataNode", 0x20C8},
		{"CVehicleSyncTree_CVehicleScriptGameStateDataNode", 0x2230},
		{"CVehicleSyncTree_CVehicleComponentReservationDataNode", 0x23A0},
		{"CVehicleSyncTree_CVehicleTaskDataNode", 0x24A8},
		{"CVehicleSyncTree_CVehicleGadgetDataNode", 0x2670},
		{"CPedSyncTree_CPedCreationDataNode", 0x3380},
		{"CPedSyncTree_CPedScriptCreationDataNode", 0x3470},
		{"CPedSyncTree_CPedSectorPosNavMeshNode", 0x3640},
		{"CPedSyncTree_CPhysicalScriptGameStateDataNode", 0x3710},
		{"CPedSyncTree_CPedInventoryDataNode", 0x3C98},
		{"CPedSyncTreeBase_CPhysicalMigrationDataNode", 0xE88},
		{"CPedSyncTreeBase_CPhysicalScriptMigrationDataNode", 0xF50},
		{"CPedSyncTreeBase_CPhysicalGameStateDataNode", 0x1020},
		{"CPedSyncTreeBase_CPedOrientationDataNode", 0x10F0},
		{"CPedSyncTreeBase_CPedMovementDataNode", 0x11B8},
		{"CPedSyncTreeBase_CPedAttachDataNode", 0x1290},
		{"CPedSyncTreeBase_CPedAIDataNode", 0x1490},
		{"CPedSyncTreeBase_CPedComponentReservationDataNode", 0x16D0},
		{"CPedSyncTreeBase_CPedHealthDataNode", 0x17D8},
		{"CPedSyncTreeBase_CPedTaskTreeDataNode", 0x18C8},
		{"CObjectSyncTree_CObjectCreationDataNode", 0x1620},
	    {"CObjectSyncTree_CObjectSectorPosNode", 0x1790},
	    {"CObjectSyncTree_CObjectOrientationNode", 0x1860},
	    {"CObjectSyncTree_CObjectScriptGameStateDataNode", 0x1AF0},
		{"CProjectSyncTree_CMigrationDataNode", 0x5E8},
	    {"CProjectSyncTree_CGlobalFlagsDataNode", 0x6B8},
	    {"CProjectSyncTree_CSectorDataNode", 0x780},
	    {"CProjectSyncTree_CSectorPositionDataNode", 0x848},
	    {"CPhysicalSyncTreeBase_CPhysicalMigrationDataNode", 0xE28},
	    {"CPhysicalSyncTreeBase_CPhysicalScriptMigrationDataNode", 0xEF0},
	    {"CPhysicalSyncTreeBase_CEntityOrientationDataNode", 0xFC0},
	    {"CPhysicalSyncTreeBase_CPhysicalVelocityDataNode", 0x10C0},
	    {"CPhysicalSyncTreeBase_CPhysicalAngVelocityDataNode", 0x1190},
	    {"CPhysicalSyncTreeBase_CPhysicalHealthDataNode", 0x1260},
	    {"CPhysicalSyncTreeBase_CPhysicalGameStateDataNode", 0x1460},
	    {"CPhysicalSyncTreeBase_CPhysicalScriptGameStateDataNode", 0x1530},
	    {"CPhysicalSyncTreeBase_CPhysicalAttachDataNode", 0x1340},
	    {"CPlayerSyncTree_CPlayerCreationDataNode", 0x3380},
	    {"CPlayerSyncTree_CPlayerSectorPosNode", 0x35D0},
	    {"CPlayerSyncTree_CPlayerGameStateDataNode", 0x37F0},
	    {"CPlayerSyncTree_CPlayerWantedAndLOSDataNode", 0x3FE0},
	    {"CPlayerSyncTree_CPlayerAmbientModelStreamingNode", 0x40E0},
	    {"CPlayerSyncTree_CPlayerCameraDataNode", 0x36D0},
	    {"CPlayerSyncTree_CPlayerAppearanceDataNode", 0x39F0},
	    {"CPlayerSyncTree_CPlayerPedGroupDataNode", 0x3E50},
	    {"CPlayerSyncTree_CPlayerGamerDataNode", 0x41B0},
	    {"CPlayerSyncTree_CPlayerExtendedGameStateNode", 0x43E8}
	};


    using get_sync_tree_for_type = uint64_t * (*)(uint64_t* ptr, uint32_t object_type);
	auto get_tree_for_type = scan("0F B7 CA 83 F9 07").as<get_sync_tree_for_type>();


enum class eSyncTree
	{
		CAutomobileSyncTree,
		CBikeSyncTree,
		CBoatSyncTree,
		CDoorSyncTree,
		CHeliSyncTree,
		CObjectSyncTree,
		CPedSyncTree,
		CPickupSyncTree,
		CPickupPlacementSyncTree,
		CPlaneSyncTree,
		CSubmarineSyncTree,
		CPlayerSyncTree,
		CTrainSyncTree
	};

inline void* get_data_node_vtable(const char* node, int tree, int received_tree)
{
	if (received_tree == tree) {
		uint64_t* base = g_patterns.m_get_sync_tree_for_type(nullptr, tree);
		if (const auto& it = DataNodeOffsets.find(node); it != DataNodeOffsets.end())
		{
			return *reinterpret_cast<void***>((uint64_t)reinterpret_cast<uint64_t*>(base) + it->second);
		}
		else
		{
			g_logger.insert(log_class::FATAL, "Failed to find data node key! ", node);
		}
	}
	return nullptr;
}

inline void* get_node_common_data_operations_reader(const char* node, int tree)
{
	uint64_t* base = g_patterns.m_get_sync_tree_for_type(nullptr, tree);
	if (const auto& it = DataNodeOffsets.find(node); it != DataNodeOffsets.end())
	{
		return (*reinterpret_cast<void***>((uint64_t)reinterpret_cast<uint64_t*>(base) + (it->second + 0xB0)))[1];
	}
	else
	{
		g_logger.insert(log_class::FATAL, "Failed to find data node key! ", node);
	}
}

inline void* get_node_common_data_operations_writer(const char* node, int tree)
{
	uint64_t* base = g_patterns.m_get_sync_tree_for_type(nullptr, tree);
	if (const auto& it = DataNodeOffsets.find(node); it != DataNodeOffsets.end())
	{
		return (*reinterpret_cast<void***>((uint64_t)reinterpret_cast<uint64_t*>(base) + (it->second + 0xB0)))[1];
	}
	else
	{
		g_logger.insert(log_class::FATAL, "Failed to find data node key! ", node);
	}
}

    bool check_node(rage::netSyncNodeBase* node,  rage::netObject* object)
	{
		if (node->IsParentNode())
		{
			for (auto child = node->m_first_child; child; child = child->m_next_sibling)
			{
				if (check_node(child, object))
					return true;
			}
		}
		else if (node->IsDataNode())
		{
			auto vtable = *(void**)node;
			const auto vft = *(uintptr_t*)node;

			if (vtable == get_data_node_vtable("CVehicleSyncTree_CVehicleCreationDataNode", (int)eSyncTree::CAutomobileSyncTree, object->m_object_type))
			{
				auto data = reinterpret_cast<CVehicleCreationDataNode*>(node);
                //do stuff
			}
		}
		return false;
	}

    bool sync_can_apply_data(rage::netSyncTree* netSyncTree, rage::netObject* netObject)
	{

		if (netSyncTree->m_child_node_count && netSyncTree->m_next_sync_node && check_node(netSyncTree->m_next_sync_node, netObject))
		{
			return false;
		}

		//return org
	}
