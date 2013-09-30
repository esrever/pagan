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
		ecs.AddComponent(entity, std::make_shared<pgn::cComponent<int>>(iComp));
		ecs.AddComponent(entity, std::make_shared<pgn::cComponent<double>>(dComp));
		ecs.AddComponent(entity, std::make_shared<pgn::cComponent<float>>(fComp));
		ecs.RemoveComponent(entity, std::make_shared<pgn::cComponent<int>>(iComp));
		ecs.RemoveComponent(entity, std::make_shared<pgn::cComponent<float>>(fComp));
		ecs.RemoveComponent(entity, std::make_shared<pgn::cComponent<double>>(dComp));
	}
	return 0;
}