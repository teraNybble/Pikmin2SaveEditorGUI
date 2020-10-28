#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include "pikSaveRW.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	bool readData();
private slots:

	//save0
	void on_checkBox_linkPoko_save0_stateChanged(int arg1);
	void on_doubleSpinBox_dayReal_save0_valueChanged(double arg1);
	void on_doubleSpinBox_pokoReal_save0_valueChanged(double arg1);
	void on_checkBox_linkDay_save0_stateChanged(int arg1);
	void on_pushButton_reRead_save0_clicked();
	void on_pushButton_write_save0_clicked();
	void on_pushButton_close_save0_clicked();
	void on_checkBox_treasureAll_save0_stateChanged(int arg1);
	//save1
	void on_checkBox_linkPoko_save1_stateChanged(int arg1);
	void on_doubleSpinBox_dayReal_save1_valueChanged(double arg1);
	void on_doubleSpinBox_pokoReal_save1_valueChanged(double arg1);
	void on_checkBox_linkDay_save1_stateChanged(int arg1);
	void on_pushButton_reRead_save1_clicked();
	void on_pushButton_write_save1_clicked();
	void on_pushButton_close_save1_clicked();
	void on_checkBox_treasureAll_save1_stateChanged(int arg1);
	//save2
	void on_checkBox_linkPoko_save2_stateChanged(int arg1);
	void on_doubleSpinBox_dayReal_save2_valueChanged(double arg1);
	void on_doubleSpinBox_pokoReal_save2_valueChanged(double arg1);
	void on_checkBox_linkDay_save2_stateChanged(int arg1);
	void on_pushButton_reRead_save2_clicked();
	void on_pushButton_write_save2_clicked();
	void on_pushButton_close_save2_clicked();
	void on_checkBox_treasureAll_save2_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
	int saves[3];
	QString savePath;
	std::fstream saveFile;

	const QString pikTypes[3] = { "Leaf", "Bud", "Flower"};
	const QString pikColours[7] = { "Blue", "Red", "Yellow", "Purple", "White", "Bulbmin", "Carrot" };
	const QString areaAbbrevations[4] = { "vor", "aw", "pp", "ww" };
	const QList<QString> names = { "Pellet Posy", "Dwarf Red Bulborb", "Red Bulborb", "Lapis Lazuli Candypop Bud", "Crimson Candypop Bud", "Golden Candypop Bud", "Violet Candypop Bud", "Ivory Candypop Bud", "Queen Candypop Bud", "Iridescent Flint Beetle", "Iridescent Glint Beetle", "Doodlebug", "Female Sheargrub", "Male Sheargrub", "Shearwig", "Cloaking Burrow-nit", "Honeywisp", "Yellow Wollywog", "Wollywog", "Falling boulder", "Fire geyser", "Gas pipe", "Electrical wire", "Swooping Snitchbug", "Fiery Blowhog", "Watery Blowhog", "Water Dumple", "Wogpole", "Anode Beetle", "Puffy Blowhog", "Empress Bulblax", "Bulborb Larva", "Bumbling Snitchbug", "Fiery Bulblax", "Burrowing Snagret", "Spotty Bulbear", "Bomb rock", "Egg", "Breadbug (with its home)", "Crashes", "Giant Breadbug and base", "Antenna Beetle", "Orange Bulborb", "Hairy Bulborb", "Dwarf Orange Bulborb", "Snow Bulborb", "Dandelion", "Clover", "Common Glowcap", "Figwort (red small)", "Figwort (red large)", "Shoot (shorter)", "Shoot (taller)", "Emperor Bulblax", "Mamuta", "Withering Blowhog", "Beady Long Legs", "Lesser Spotted Jellyfloat", "Careening Dirigibug", "Fiery Dweevil", "Caustic Dweevil", "Munge Dweevil", "Anode Dweevil", "Hermit Crawmad", "Crashes, unused", "Ravenous Whiskerpillar", "Man-at-Legs", "Bulbmin", "Mitite", "Raging Long Legs", "Pileated Snagret", "Ranging Bloyster", "Greater Spotted Jellyfloat", "Titan Dweevil", "Rock", "Armored Cannon Beetle Larva", "Dwarf Bulbear", "Group of 9 Unmarked Spectralids", "Gatling Groink", "Skitter Leaf", "Horsetail", "Seeding Dandelion", "Crashes, Candypop Bud base", "Breadbug's Home?[unsure]", "Creeping Chrysanthemum", "Glowstem (red)", "Glowstem (green)", "Margaret", "Foxtail", "Chigoyami paper", "Fiddlehead", "Figwort (brown small)", "Figwort (brown large)", "Volatile Dweevil", "Segmented Crawbster", "Decorated Cannon Beetle", "Armored Cannon Beetle Larva (burrowed)", "Gatling Groink (pedestal)", "Waterwraith rollers", "Waterwraith", "Crashes, Bloyster base", "Toady Bloyster" };
	const QList<QString> enyNames = { "PelletPosy", "DwarfRedBulborbb", "RedBulborb", "LapisLazuliCandypopBudud", "CrimsonCandypopBudd", "GoldenCandypopBudd", "VioletCandypopBudd", "IvoryCandypopBudd", "QueenCandypopBudd", "IridescentFlintBeetlee", "IridescentGlintBeetlee", "Doodlebug", "FemaleSheargrub", "MaleSheargrub", "Shearwig", "CloakingBurrownit", "Honeywisp", "YellowWollywog", "Wollywog", "Fallingboulder", "Firegeyser", "Gaspipe", "Electricalwire", "SwoopingSnitchbug", "FieryBlowhog", "WateryBlowhog", "WaterDumple", "Wogpole", "AnodeBeetle", "PuffyBlowhog", "EmpressBulblax", "BulborbLarva", "BumblingSnitchbug", "FieryBulblax", "BurrowingSnagret", "SpottyBulbear", "Bombrock", "Egg", "Breadbugwithitshomeme", "Crashes", "GiantBreadbugandbasese", "AntennaBeetle", "OrangeBulborb", "HairyBulborb", "DwarfOrangeBulborbb", "SnowBulborb", "Dandelion", "Clover", "CommonGlowcap", "Figwortredsmalll", "Figwortredlargee", "Shootshorter", "Shoottaller", "EmperorBulblax", "Mamuta", "WitheringBlowhog", "BeadyLongLegss", "LesserSpottedJellyfloatt", "CareeningDirigibug", "FieryDweevil", "CausticDweevil", "MungeDweevil", "AnodeDweevil", "HermitCrawmad", "Crashesunused", "RavenousWhiskerpillar", "ManatLegss", "Bulbmin", "Mitite", "RagingLongLegss", "PileatedSnagret", "RangingBloyster", "GreaterSpottedJellyfloatt", "TitanDweevil", "Rock", "ArmoredCannonBeetleLarvava", "DwarfBulbear", "Groupof9UnmarkedSpectralidsids", "GatlingGroink", "SkitterLeaf", "Horsetail", "SeedingDandelion", "CrashesCandypopBudbasese", "BreadbugsHomeunsure", "CreepingChrysanthemum", "Glowstemred", "Glowstemgreen", "Margaret", "Foxtail", "Chigoyamipaper", "Fiddlehead", "Figwortbrownsmalll", "Figwortbrownlargee", "VolatileDweevil", "SegmentedCrawbster", "DecoratedCannonBeetlee", "ArmoredCannonBeetleLarvaburrowedwed", "GatlingGroinkpedestall", "Waterwraithrollers", "Waterwraith", "CrashesBloysterbasee", "ToadyBloyster" };
	const QList<QString> treasureNames = {"RubberUgly","InsectCondo","MeatSatchel","CoiledLauncher","ConfectionHoop","OmniscientSphere","LoveSphere","MirthSphere","MaternalSculpture","MysteriousRemains","LeviathanFeather","SuperstrongStabilizer","SpaceWaveReceiver","JoyReceptacle","WorthlessStatue","PricelessStatue","TripleSugarThreat","KingofSweets","DietDoomer","PalePassion","MagicalStage","BugBait","MilkTub","PetrifiedHeart","RegalDiamond","PrincessPearl","Silencer","ArmoredNut","ChocolateCushion","SweetDreamer","CosmicArchive","CupidsGrenade","ScienceProject","ManualHoner","MercilessExtractor","SudGenerator","WiggleNoggin","OmegaFlywheel","LustrousElement","SuperstickTextile","PossessedSquash","GyroidBust","SunseedBerry","GleeSpinner","DecorativeGoo","AntihiccupFungus","CrystalKing","FossilizedUrsidae","TimeCapsule","OlimarniteShell","ConiferSpire","PlentifulTank","AbroralFrippery","OnionReplica","InfernalVegetable","AdamantineGirdle","DirectoryofDestiny","ColossalFossil","Invigorator","VacuumProcessor","MirroredElement","NouveauTable","PinkMenace","FrostyBauble","GemstarHusband","GemstarWife","UniversalCom","JoylessJewel","FleetingArtForm","InnocenceLost","IconofProgress","UnspeakableWonder","AquaticMine","TemporalMechanism","EssentialFurnishing","FlameTiller","DoomsdayApparatus","ImpedimentScourge","FutureOrb","ShockTherapist","FlareCannon","ComedyBomb","MonsterPump","MysticalDisc","VorpalPlatter","TasteSensation","LipService","UtterScrap","ParadoxicalEnigma","KingofBugs","EssenceofRage","EssenceofDespair","EssenceofTrueLove","EssenceofDesire","CitrusLump","BehemothJaw","AnxiousSprout","ImplementofToil","LuckWafer","MeatofChampions","TalismanofLife","StrifeMonolith","BossStone","ToxicToadstool","Growshroom","IndomitableCPU","NetworkMainbrain","RepairJuggernaut","ExhaustedSuperstick","PastryWheel","CombustionBerry","ImperativeCookie","CompellingCookie","ImpenetrableCookie","ComfortCookie","SucculentMattress","CorpulentNut","NutrientSilo","MassageGirdle","CrystallizedTelepathy","CrystallizedTelekinesis","CrystallizedClairvoyance","EternalEmeraldEye","TearStone","CrystalClover","DangerChime","SulkingAntenna","SpouseAlert","MastersInstrument","ExtremePerspirator","PilgrimBulb","StoneofGlory","FuriousAdhesive","QuenchingEmblem","FlameofTomorrow","LoveNugget","ChildoftheEarth","DisguisedDelicacy","ProtonAA","FuelReservoir","AbstractMasterpiece","DurableEnergyCell","CourageReactor","PerfectContainer","HarmonicSynthesizer","DivineCookingTool","RememberedOldBuddy","FondGyroBlock","MemorableGyroBlock","LostGyroBlock","FavoriteGyroBlock","TreasuredGyroBlock","FortifiedDelicacy","ScrumptiousShell","MemorialShell","ChanceTotem","DreamArchitect","SpinyAlienTreat","SpiritFlogger","MirroredStage","EnamelBuster","DroughtEnder","WhiteGoodness","Salivatrix","ActivityArouser","DroneSupplies","SurvivalContainer","ContainerofSeaBounty","ContainerofKnowledge","PatienceTester","YellowTasteTyrant","AlienBillboard","GherkinGate","HypnoticPlatter","HapinessEmblem","PonderingEmblem","EndlessRepository","OpenArchitecture","PermanentContainer","EmptySpaceContainer","StringentContainer","OpenArchive","UnknownMerit","SeedofGreed","HeavyDutyMagnetizer","AirBrake","HideousVictual","EmperorWhistle"};
	const QList<QString> exNames = { "BruteKnuckles", "DreamMaterial", "AmplifiedAmplifier", "ProfessionalNoisemaker", "StellarOrb", "JusticeAlloy", "ForgedCourage", "RepugnantAppendage", "PrototypeDetector", "FivemanNapsack", "SphericalAtlas", "GeographicProjection", "TheKey" };
	int (* areaProgressReadFuncs[4])(std::fstream&,int) = { &readValleyOfReposeProgress, &readAwakeningWoodProgress, &readPerplexingPoolProgress, &readWistfulWildProgress };
	void (* areaProgressWriteFuncs[4])(std::fstream&,int,int) = { &writeValleyOfReposeProgress, &writeAwakeningWoodProgress, &writePerplexingPoolProgress, &writeWistfulWildProgress };

	void disableButtons(int saveNo);
	void enableButtons(int saveNo);
	void writeToSave(int saveNo);
};
#endif // MAINWINDOW_H
