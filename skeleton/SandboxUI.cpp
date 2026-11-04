#include "SandboxUI.h"

#include "SistemaParticulas.h"
#include "ParticleGenerator.h"
#include "FountainParticleGenerator.h"
#include "FireParticleGenerator.h"
#include "FogParticleGenerator.h"
#include "ForceGenerator.h"
#include "PositionalForceGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "BuoyancyForceGenerator.h"

#include "imgui.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace
{
	bool DragVec3(const char* label, Vector3D& value, float speed = 0.1f,
		float minValue = -10000.0f, float maxValue = 10000.0f)
	{
		float components[3] = { value.x, value.y, value.z };
		if (ImGui::DragFloat3(label, components, speed, minValue, maxValue, "%.3f"))
		{
			value = Vector3D(components[0], components[1], components[2]);
			return true;
		}
		return false;
	}

	// Evita que una desviación <= 0 llegue a std::normal_distribution.
	bool DragPositiveVec3(const char* label, Vector3D& value, float speed = 0.1f)
	{
		const float minValue = 0.001f;
		Vector3D sanitized(
			std::fmax(value.x, minValue),
			std::fmax(value.y, minValue),
			std::fmax(value.z, minValue));

		bool changed = sanitized.x != value.x ||
			sanitized.y != value.y ||
			sanitized.z != value.z;

		if (DragVec3(label, sanitized, speed, minValue))
		{
			changed = true;
		}

		if (changed)
		{
			value = sanitized;
			return true;
		}
		return false;
	}

	bool ColorEdit4(const char* label, Vector4& value)
	{
		float components[4] = { value.x, value.y, value.z, value.w };
		if (ImGui::ColorEdit4(label, components, ImGuiColorEditFlags_AlphaBar))
		{
			value = Vector4(components[0], components[1], components[2], components[3]);
			return true;
		}
		return false;
	}

	const char* GeneratorTypeName(ParticleGeneratorType type)
	{
		switch (type)
		{
		case ParticleGeneratorType::FOUNTAIN: return "Fountain";
		case ParticleGeneratorType::FIRE: return "Fire";
		case ParticleGeneratorType::FOG: return "Fog";
		default: return "Particle Generator";
		}
	}

	const char* ForceTypeName(ForceGeneratorType type)
	{
		switch (type)
		{
		case ForceGeneratorType::GRAVITY: return "Gravity";
		case ForceGeneratorType::WIND: return "Wind";
		case ForceGeneratorType::WHIRLWIND: return "Whirlwind";
		case ForceGeneratorType::EXPLOSION: return "Explosion";
		case ForceGeneratorType::BUOYANCY: return "Buoyancy";
		case ForceGeneratorType::ANCHORED_SPRING: return "Anchored Spring";
		case ForceGeneratorType::SPRING: return "Spring";
		case ForceGeneratorType::RUBBER: return "Elastic Rubber";
		default: return "Force Generator";
		}
	}

	void DrawPositionalFields(PositionalForceGenerator* generator)
	{
		Vector3D origin = generator->getOrigin();
		if (DragVec3("Zone Origin", origin))
		{
			generator->setOrigin(origin);
		}

		Vector3D halfExtents = generator->getHalfExtents();
		if (DragVec3("Half Extents", halfExtents))
		{
			halfExtents.x = std::fmax(0.1f, halfExtents.x);
			halfExtents.y = std::fmax(0.1f, halfExtents.y);
			halfExtents.z = std::fmax(0.1f, halfExtents.z);
			generator->setHalfExtents(halfExtents);
		}

		bool debug = generator->isDebug();
		if (ImGui::Checkbox("Show Debug Zone", &debug))
		{
			generator->setDebug(debug);
		}

		Vector4 debugColor = generator->getDebugColor();
		if (ColorEdit4("Zone Color", debugColor))
		{
			generator->setDebugColor(debugColor);
		}
	}
}

SandboxUI::SandboxUI(SistemaParticulas& system)
	: m_system(system)
{
}

SandboxUI::~SandboxUI() = default;

void SandboxUI::draw(bool& paused)
{
	ImGui::SetNextWindowSize(ImVec2(430.0f, 620.0f), ImGuiCond_FirstUseEver);
	ImGui::Begin("Sandbox");

	if (ImGui::BeginTabBar("SandboxTabs"))
	{
		if (ImGui::BeginTabItem("Scene"))
		{
			drawSceneTab(paused);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Particle Generators"))
		{
			drawGeneratorsTab();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Force Generators"))
		{
			drawForceGeneratorsTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
	// Se aplican al final para no modificar los vectores mientras se recorren.
	flushMutations();
}

void SandboxUI::drawSceneTab(bool& paused)
{
	ImGui::Text("Particles: %zu", m_system.getParticleCount());
	ImGui::Text("Particle generators: %zu", m_system.getGenerators().size());
	ImGui::Text("Force generators: %zu", m_system.getForceGenerators().size());
	ImGui::Separator();

	if (ImGui::Checkbox("Pause Simulation", &paused))
	{
	}

	if (ImGui::Button("Clear All Particles"))
	{
		m_system.clearParticles();
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear Scene"))
	{
		m_system.cleanup();
	}

	if (ImGui::Button("Load Demo Scene"))
	{
		seedDemo();
	}

	ImGui::Separator();
	ImGui::TextWrapped("Create and edit particle generators and force generators from the other tabs. Changes are applied at the end of the ImGui frame.");
}

void SandboxUI::drawGeneratorsTab()
{
	ImGui::TextUnformatted("Create Particle Generator");
	const char* generatorTypes[] = { "Fountain", "Fire", "Fog" };
	ImGui::Combo("Type##new_generator", &m_generatorTypeToAdd, generatorTypes, 3);
	DragVec3("Spawn Position##new_generator", m_newGeneratorOrigin);

	if (ImGui::Button("Add Particle Generator"))
	{
		std::unique_ptr<ParticleGenerator> generator;
		switch (m_generatorTypeToAdd)
		{
		case 0:
			generator = std::make_unique<FountainParticleGenerator>(m_newGeneratorOrigin);
			break;
		case 1:
			generator = std::make_unique<FireParticleGenerator>(m_newGeneratorOrigin);
			break;
		case 2:
		{
			Vector3D halfBox(6.0f, 2.0f, 6.0f);
			generator = std::make_unique<FogParticleGenerator>(
				m_newGeneratorOrigin - halfBox,
				m_newGeneratorOrigin + halfBox);
			break;
		}
		default:
			break;
		}

		if (generator)
		{
			m_pendingGenerators.push_back(std::move(generator));
		}
	}

	ImGui::Separator();
	ImGui::Text("Particle Generators (%zu)", m_system.getGenerators().size());

	if (m_system.getGenerators().empty())
	{
		ImGui::TextDisabled("No particle generators yet.");
	}

	int index = 0;
	for (const auto& generatorPtr : m_system.getGenerators())
	{
		ParticleGenerator* generator = generatorPtr.get();
		++index;

		ImGui::PushID(static_cast<const void*>(generator));
		char header[128];
		std::snprintf(header, sizeof(header), "%s #%d", GeneratorTypeName(generator->getType()), index);

		if (ImGui::CollapsingHeader(header, ImGuiTreeNodeFlags_DefaultOpen))
		{
			drawGeneratorInspector(generator);

			ImGui::Spacing();
			if (ImGui::Button("Delete"))
			{
				m_pendingGeneratorDeletes.push_back(generator);
			}
		}
		ImGui::PopID();
	}
}

void SandboxUI::drawGeneratorInspector(ParticleGenerator* generator)
{
	bool active = generator->isActive();
	if (ImGui::Checkbox("Active", &active))
	{
		generator->setActive(active);
	}

	Vector3D origin = generator->getOrigin();
	if (DragVec3("Origin", origin))
	{
		generator->setOrigin(origin);
	}

	const char* colorModes[] = { "Single Color", "Color Distribution" };
	int colorMode = static_cast<int>(generator->getColorMode());
	if (ImGui::Combo("Color Mode", &colorMode, colorModes, 2))
	{
		generator->setColorMode(static_cast<ParticleColorMode>(colorMode));
	}

	if (generator->getColorMode() == ParticleColorMode::SINGLE)
	{
		Vector4 color = generator->getColor();
		if (ColorEdit4("Color", color))
		{
			generator->setColor(color);
		}
	}
	else
	{
		Vector4 minColor = generator->getColorMin();
		if (ColorEdit4("Min Color", minColor))
		{
			generator->setColorRange(minColor, generator->getColorMax());
		}

		Vector4 maxColor = generator->getColorMax();
		if (ColorEdit4("Max Color", maxColor))
		{
			generator->setColorRange(generator->getColorMin(), maxColor);
		}
	}

	float radius = generator->getRadius();
	if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.01f, 10.0f, "%.3f"))
	{
		generator->setRadius(radius);
	}

	float mass = generator->getMass();
	if (ImGui::DragFloat("Mass", &mass, 0.05f, 0.01f, 1000.0f, "%.3f"))
	{
		generator->setMass(mass);
	}

	float damping = generator->getDamping();
	if (ImGui::DragFloat("Damping", &damping, 0.001f, 0.0f, 1.0f, "%.3f"))
	{
		generator->setDamping(damping);
	}

	float lifetime = generator->getLifetime();
	if (ImGui::DragFloat("Lifetime", &lifetime, 0.05f, 0.1f, 120.0f, "%.2f s"))
	{
		generator->setLifetime(lifetime);
	}

	bool scaleWithLifetime = generator->getScaleWithLifetime();
	if (ImGui::Checkbox("Scale With Lifetime", &scaleWithLifetime))
	{
		generator->setScaleWithLifetime(scaleWithLifetime);
	}

	int particlesPerSpawn = generator->getParticlesPerSpawn();
	if (ImGui::DragInt("Particles / Spawn", &particlesPerSpawn, 1, 1, 100))
	{
		generator->setEmissionRate(particlesPerSpawn, generator->getSpawnInterval());
	}

	float spawnInterval = static_cast<float>(generator->getSpawnInterval());
	if (ImGui::DragFloat("Spawn Interval", &spawnInterval, 0.001f, 0.001f, 5.0f, "%.3f s"))
	{
		generator->setSpawnInterval(spawnInterval);
	}

	int maxParticles = static_cast<int>(generator->getMaxParticles());
	if (ImGui::DragInt("Max Particles", &maxParticles, 10, 1, 10000))
	{
		if (maxParticles < 1)
		{
			maxParticles = 1;
		}
		generator->setMaxParticles(static_cast<size_t>(maxParticles));
	}

	ImGui::Text("Pool: %zu / %zu", generator->getPoolSize(), generator->getMaxParticles());

	switch (generator->getType())
	{
	case ParticleGeneratorType::FOUNTAIN:
	{
		FountainParticleGenerator* fountain = static_cast<FountainParticleGenerator*>(generator);
		Vector3D meanVelocity = fountain->getMeanVelocity();
		if (DragVec3("Mean Velocity", meanVelocity))
		{
			fountain->setMeanVelocity(meanVelocity);
		}

		Vector3D stdDevVelocity = fountain->getStdDevVelocity();
		if (DragPositiveVec3("Velocity Std Dev", stdDevVelocity, 0.05f))
		{
			fountain->setStdDevVelocity(stdDevVelocity);
		}
		break;
	}
	case ParticleGeneratorType::FIRE:
	{
		FireParticleGenerator* fire = static_cast<FireParticleGenerator*>(generator);
		Vector3D meanVelocity = fire->getMeanVelocity();
		if (DragVec3("Mean Velocity", meanVelocity))
		{
			fire->setMeanVelocity(meanVelocity);
		}

		Vector3D stdDevVelocity = fire->getStdDevVelocity();
		if (DragPositiveVec3("Velocity Std Dev", stdDevVelocity, 0.05f))
		{
			fire->setStdDevVelocity(stdDevVelocity);
		}
		break;
	}
	case ParticleGeneratorType::FOG:
	{
		FogParticleGenerator* fog = static_cast<FogParticleGenerator*>(generator);
		Vector3D minBox = fog->getMinBox();
		Vector3D maxBox = fog->getMaxBox();
		bool changed = DragVec3("Min Box", minBox);
		changed = DragVec3("Max Box", maxBox) || changed;

		if (changed)
		{
			maxBox.x = std::fmax(maxBox.x, minBox.x + 0.1f);
			maxBox.y = std::fmax(maxBox.y, minBox.y + 0.1f);
			maxBox.z = std::fmax(maxBox.z, minBox.z + 0.1f);
			fog->setBounds(minBox, maxBox);
		}
		break;
	}
	default:
		break;
	}
}

void SandboxUI::drawForceGeneratorsTab()
{
	ImGui::TextUnformatted("Create Force Generator");
	const char* forceTypes[] = { "Gravity", "Wind", "Whirlwind", "Explosion", "Buoyancy" };
	ImGui::Combo("Type##new_force", &m_forceGeneratorTypeToAdd, forceTypes, 5);
	DragVec3("Zone Center##new_force", m_newForceOrigin);

	if (ImGui::Button("Add Force Generator"))
	{
		std::unique_ptr<ForceGenerator> forceGenerator;
		switch (m_forceGeneratorTypeToAdd)
		{
		case 0:
		{
			auto gravity = std::make_unique<GravityForceGenerator>(
				m_newForceOrigin,
				Vector3D(10.0f, 10.0f, 10.0f),
				Vector3D(0.0f, -9.8f, 0.0f));
			gravity->setDebug(true);
			forceGenerator = std::move(gravity);
			break;
		}
		case 1:
		{
			auto wind = std::make_unique<WindForceGenerator>(
				m_newForceOrigin,
				Vector3D(10.0f, 10.0f, 10.0f),
				Vector3D(10.0f, 0.0f, 0.0f),
				1.0f);
			wind->setDebug(true);
			forceGenerator = std::move(wind);
			break;
		}
		case 2:
		{
			auto whirlwind = std::make_unique<WhirlwindForceGenerator>(
				m_newForceOrigin,
				10.0f,
				20.0f,
				2.0f,
				1.5f);
			whirlwind->setDebug(true);
			forceGenerator = std::move(whirlwind);
			break;
		}
		case 3:
		{
			forceGenerator = std::make_unique<ExplosionForceGenerator>(
				m_newForceOrigin,
				2000.0f,
				30.0f,
				0.4f);
			break;
		}
		case 4:
		{
			forceGenerator = std::make_unique<BuoyancyForceGenerator>(
				2.0f,
				2.0f,
				1000.0f,
				5.0f,
				nullptr,
				nullptr);
			break;
		}
		default:
			break;
		}

		if (forceGenerator)
		{
			m_pendingForceGenerators.push_back(std::move(forceGenerator));
		}
	}

	ImGui::Separator();
	ImGui::Text("Force Generators (%zu)", m_system.getForceGenerators().size());

	if (m_system.getForceGenerators().empty())
	{
		ImGui::TextDisabled("No force generators yet.");
	}

	int index = 0;
	for (const auto& forceGeneratorPtr : m_system.getForceGenerators())
	{
		ForceGenerator* forceGenerator = forceGeneratorPtr.get();
		++index;

		ImGui::PushID(static_cast<const void*>(forceGenerator));
		char header[128];
		std::snprintf(header, sizeof(header), "%s #%d", ForceTypeName(forceGenerator->getType()), index);

		if (ImGui::CollapsingHeader(header, ImGuiTreeNodeFlags_DefaultOpen))
		{
			drawForceGeneratorInspector(forceGenerator);

			ImGui::Spacing();
			if (ImGui::Button("Delete"))
			{
				m_pendingForceGeneratorDeletes.push_back(forceGenerator);
			}
		}
		ImGui::PopID();
	}
}

void SandboxUI::drawForceGeneratorInspector(ForceGenerator* generator)
{
	switch (generator->getType())
	{
	case ForceGeneratorType::GRAVITY:
	{
		GravityForceGenerator* gravity = static_cast<GravityForceGenerator*>(generator);
		DrawPositionalFields(gravity);

		Vector3D gravityValue = gravity->getGravity();
		if (DragVec3("Gravity", gravityValue))
		{
			gravity->setGravity(gravityValue);
		}
		break;
	}
	case ForceGeneratorType::WIND:
	{
		WindForceGenerator* wind = static_cast<WindForceGenerator*>(generator);
		DrawPositionalFields(wind);

		Vector3D windVelocity = wind->getWindVelocity();
		if (DragVec3("Wind Velocity", windVelocity))
		{
			wind->setWindVelocity(windVelocity);
		}

		float k1 = wind->getK1();
		if (ImGui::DragFloat("k1", &k1, 0.01f, 0.0f, 100.0f, "%.3f"))
		{
			wind->setK1(k1);
		}
		break;
	}
	case ForceGeneratorType::WHIRLWIND:
	{
		WhirlwindForceGenerator* whirlwind = static_cast<WhirlwindForceGenerator*>(generator);

		Vector3D center = whirlwind->getOrigin();
		if (DragVec3("Center", center))
		{
			whirlwind->setOrigin(center);
		}

		float radius = whirlwind->getRadius();
		if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.1f, 200.0f, "%.2f"))
		{
			whirlwind->setRadius(radius);
		}

		float height = whirlwind->getHeight();
		if (ImGui::DragFloat("Height", &height, 0.1f, 0.1f, 400.0f, "%.2f"))
		{
			whirlwind->setHeight(height);
		}

		float K = whirlwind->getK();
		if (ImGui::DragFloat("K", &K, 0.01f, 0.0f, 100.0f, "%.3f"))
		{
			whirlwind->setK(K);
		}

		float k1 = whirlwind->getK1();
		if (ImGui::DragFloat("k1", &k1, 0.01f, 0.0f, 100.0f, "%.3f"))
		{
			whirlwind->setK1(k1);
		}

		bool debug = whirlwind->isDebug();
		if (ImGui::Checkbox("Show Debug Zone", &debug))
		{
			whirlwind->setDebug(debug);
		}

		Vector4 debugColor = whirlwind->getDebugColor();
		if (ColorEdit4("Zone Color", debugColor))
		{
			whirlwind->setDebugColor(debugColor);
		}
		break;
	}
	case ForceGeneratorType::EXPLOSION:
	{
		ExplosionForceGenerator* explosion = static_cast<ExplosionForceGenerator*>(generator);

		Vector3D center = explosion->getCenter();
		if (DragVec3("Center", center))
		{
			explosion->setCenter(center);
		}

		float K = explosion->getK();
		if (ImGui::DragFloat("K", &K, 10.0f, 0.0f, 100000.0f, "%.1f"))
		{
			explosion->setK(K);
		}

		float radius = explosion->getRadius();
		if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.1f, 500.0f, "%.2f"))
		{
			explosion->setRadius(radius);
		}

		float tau = explosion->getTau();
		if (ImGui::DragFloat("Tau", &tau, 0.01f, 0.01f, 10.0f, "%.3f"))
		{
			explosion->setTau(tau);
		}

		bool expandingWave = explosion->isExpandingWave();
		if (ImGui::Checkbox("Expanding Wave", &expandingWave))
		{
			explosion->setExpandingWave(expandingWave);
		}

		if (expandingWave)
		{
			float expansionSpeed = explosion->getExpansionSpeed();
			if (ImGui::DragFloat("Expansion Speed", &expansionSpeed, 1.0f, 1.0f, 2000.0f, "%.1f"))
			{
				explosion->setExpansionSpeed(expansionSpeed);
			}
		}

		if (ImGui::Button("Trigger Explosion"))
		{
			explosion->trigger();
		}

		ImGui::SameLine();
		ImGui::Text(explosion->isActive() ? "Active" : "Idle");
		break;
	}
	case ForceGeneratorType::BUOYANCY:
	{
		BuoyancyForceGenerator* buoyancy = static_cast<BuoyancyForceGenerator*>(generator);

		float height = buoyancy->getHeight();
		if (ImGui::DragFloat("Object Height", &height, 0.05f, 0.01f, 100.0f, "%.3f"))
		{
			buoyancy->setHeight(height);
		}

		float volume = buoyancy->getVolume();
		if (ImGui::DragFloat("Volume", &volume, 0.05f, 0.01f, 1000.0f, "%.3f"))
		{
			buoyancy->setVolume(volume);
		}

		float density = buoyancy->getLiquidDensity();
		if (ImGui::DragFloat("Liquid Density", &density, 1.0f, 0.0f, 20000.0f, "%.1f"))
		{
			buoyancy->setLiquidDensity(density);
		}

		float waterHeight = buoyancy->getWaterHeight();
		if (ImGui::DragFloat("Water Height", &waterHeight, 0.05f, -100.0f, 500.0f, "%.2f"))
		{
			buoyancy->setWaterHeight(waterHeight);
		}

		bool enabled = buoyancy->isEnabled();
		if (ImGui::Checkbox("Enabled", &enabled))
		{
			buoyancy->setEnabled(enabled);
		}

		ImGui::TextDisabled("Currently applies to every particle.");
		break;
	}
	default:
		ImGui::TextDisabled("This force generator has no runtime editor.");
		break;
	}
}

void SandboxUI::flushMutations()
{
	for (auto& generator : m_pendingGenerators)
	{
		m_system.addGenerator(std::move(generator));
	}
	m_pendingGenerators.clear();

	for (auto& forceGenerator : m_pendingForceGenerators)
	{
		m_system.addForceGenerator(std::move(forceGenerator));
	}
	m_pendingForceGenerators.clear();

	for (ParticleGenerator* generator : m_pendingGeneratorDeletes)
	{
		m_system.removeGenerator(generator);
	}
	m_pendingGeneratorDeletes.clear();

	for (ForceGenerator* forceGenerator : m_pendingForceGeneratorDeletes)
	{
		m_system.removeForceGenerator(forceGenerator);
	}
	m_pendingForceGeneratorDeletes.clear();
}

void SandboxUI::seedDemo()
{
	m_system.cleanup();

	auto gravity = std::make_unique<GravityForceGenerator>(
		Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D(60.0f, 20.0f, 60.0f),
		Vector3D(0.0f, -9.8f, 0.0f),
		Vector4(0.2f, 0.6f, 1.0f, 1.0f));
	gravity->setDebug(true);
	m_system.addForceGenerator(std::move(gravity));

	auto wind = std::make_unique<WindForceGenerator>(
		Vector3D(20.0f, 10.0f, 0.0f),
		Vector3D(10.0f, 15.0f, 10.0f),
		Vector3D(15.0f, 0.0f, 0.0f),
		1.2f,
		Vector4(0.2f, 0.9f, 0.3f, 1.0f));
	wind->setDebug(true);
	m_system.addForceGenerator(std::move(wind));

	auto whirlwind = std::make_unique<WhirlwindForceGenerator>(
		Vector3D(-20.0f, 10.0f, 0.0f),
		8.0f,
		25.0f,
		2.0f,
		1.5f,
		Vector4(0.8f, 0.4f, 1.0f, 1.0f));
	whirlwind->setDebug(true);
	m_system.addForceGenerator(std::move(whirlwind));

	auto fountain = std::make_unique<FountainParticleGenerator>(
		Vector3D(0.0f, 0.5f, 0.0f),
		Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D(2.0f, 2.0f, 2.0f));
	fountain->setColor(Vector4(0.2f, 0.6f, 1.0f, 1.0f));
	m_system.addGenerator(std::move(fountain));

	auto fire = std::make_unique<FireParticleGenerator>(Vector3D(12.0f, 0.5f, 0.0f));
	m_system.addGenerator(std::move(fire));

	auto fog = std::make_unique<FogParticleGenerator>(
		Vector3D(-12.0f, 0.5f, -5.0f),
		Vector3D(-2.0f, 4.0f, 5.0f));
	m_system.addGenerator(std::move(fog));

	auto explosion = std::make_unique<ExplosionForceGenerator>(
		Vector3D(0.0f, 5.0f, 0.0f),
		3000.0f,
		30.0f,
		0.4f);
	m_system.addForceGenerator(std::move(explosion));
}
