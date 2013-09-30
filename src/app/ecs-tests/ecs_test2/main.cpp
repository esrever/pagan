#include <ecs/EntityMgr.h>
#include <ecs/Component.h>

int main()
{
	auto ecs = ECS;
	pgn::cComponent<int> iComp;
	pgn::cComponent<double> dComp;
	pgn::cComponent<float> fComp;

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