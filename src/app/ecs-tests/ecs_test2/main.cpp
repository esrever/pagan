#include <ecs/EntityMgr.h>
#include <ecs/Component.h>

int main()
{
	auto ecs = ECS;
	pgn::cComponent<int> iComp;  iComp.mData = 2;
	pgn::cComponent<double> dComp; dComp.mData = 3.0;
	pgn::cComponent<float> fComp; fComp.mData= 4.0f;

	{
		auto entity = ecs.Create();
		ecs.AddComponent(entity, iComp);
		ecs.AddComponent(entity, dComp);
		ecs.AddComponent(entity, fComp);
		ecs.RemoveComponent(entity, iComp);
		ecs.RemoveComponent(entity, fComp);
		ecs.RemoveComponent(entity, dComp);
	}
	return 0;
}