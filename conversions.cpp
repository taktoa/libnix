
void nix::builtinFetchurl(const BasicDerivation& drv, const std::string& netrcData);


constructor nix::Key(const std::string& s);

struct SecretKey : Key {
    nix::SecretKey::SecretKey(const std::string& s);
    std::string nix::SecretKey::signDetached(const std::string& s) const;
    nix::PublicKey nix::SecretKey::toPublicKey() const;
};

struct PublicKey : Key {
    nix::PublicKey::PublicKey(const std::string& data);
};

typedef std::map<std::string, PublicKey> nix::PublicKeys;
bool nix::verifyDetached(const std::string& data, const std::string& sig, const nix::PublicKeys& publicKeys);
nix::PublicKeys nix::getDefaultPublicKeys();

nix::DerivationOutput::DerivationOutput();
nix::DerivationOutput::DerivationOutput(Path path, string hashAlgo, string hash);

void nix::DerivationOutput::parseHashInfo(bool & recursive, Hash & hash) const;
typedef std::map<string, DerivationOutput> nix::DerivationOutputs;
typedef std::map<Path, StringSet> nix::DerivationInputs;
typedef std::map<string, string> nix::StringPairs;
struct nix::BasicDerivation {
    Path nix::BasicDerivation::findOutput(const string & id) const;
    bool nix::BasicDerivation::willBuildLocally() const;
    bool nix::BasicDerivation::substitutesAllowed() const;
    bool nix::BasicDerivation::isBuiltin() const;
    bool nix::BasicDerivation::canBuildLocally() const;
    bool nix::BasicDerivation::isFixedOutput() const;
    PathSet nix::BasicDerivation::outputPaths() const;
};
struct nix::Derivation : nix::BasicDerivation
{
    nix::Derivation::DerivationInputs inputDrvs;
    std::string nix::Derivation::unparse() const;
};
Path nix::writeDerivation(ref<Store> store,
    const nix::Derivation & drv, const string & name, bool repair = false);
nix::Derivation nix::readDerivation(const Path & drvPath);
bool nix::isDerivation(const string & fileName);
Hash nix::hashDerivationModulo(Store & store, nix::Derivation drv);
typedef std::map<Path, Hash> nix::DrvHashes;
extern nix::DrvHashes drvHashes;
typedef std::pair<string, std::set<string> > nix::DrvPathWithOutputs;
nix::DrvPathWithOutputs nix::parseDrvPathWithOutputs(const string & s);
Path nix::makeDrvPathWithOutputs(const Path & drvPath, const std::set<string> & outputs);
bool nix::wantOutput(const string & output, const std::set<string> & wanted);
struct nix::Source;
struct nix::Sink;
nix::Source & nix::readDerivation(nix::Source & in, nix::Store & store, nix::BasicDerivation & drv);
nix::Sink & operator << (nix::Sink & out, const nix::BasicDerivation & drv);
std::string nix::hashPlaceholder(const std::string & outputName);

struct nix::DownloadRequest
{
    std::string uri;
    nix::DownloadRequest(const std::string & uri) : uri(uri);
};
struct nix::DownloadResult { };
struct nix::Downloader
{
    std::future<nix::DownloadResult> nix::Downloader::enqueueDownload(const nix::DownloadRequest & request);
    nix::DownloadResult nix::Downloader::download(const nix::DownloadRequest & request);
    Path nix::Downloader::downloadCached(ref<Store> store, const string & uri, bool unpack, string name = "", const Hash & expectedHash = Hash(), string * effectiveUri = nullptr);
};
ref<nix::Downloader> nix::getDownloader();
ref<nix::Downloader> nix::makeDownloader();
class nix::DownloadError : public Error
{
    nix::DownloadError(nix::Downloader::Error error, const FormatOrString & fs);
    bool nix::isUri(const string & s);
}

struct nix::Settings {
    typedef std::map<string, string> nix::SettingsMap;
    void nix::loadConfFile();
    void nix::set(const string & name, const string & value);
    string nix::get(const string & name, const string & def);
    Strings nix::get(const string & name, const Strings & def);
    bool nix::get(const string & name, bool def);
    int nix::get(const string & name, int def)
    void nix::update();
    string nix::pack();
    void nix::unpack(const string & pack);
    SettingsMap nix::getOverrides();
}

struct nix::Derivation;
struct nix::OptimiseStats
{
    nix::OptimiseStats();
};
class nix::LocalStore : public LocalFSStore
{
    nix::LocalStore(const Params & params);
    ~nix::LocalStore();
    std::string nix::LocalStore::getUri() override;
    bool nix::LocalStore::isValidPathUncached(const Path & path) override;
    PathSet nix::LocalStore::queryValidPaths(const PathSet & paths) override;
    PathSet nix::LocalStore::queryAllValidPaths() override;
    void nix::LocalStore::queryPathInfoUncached(const Path & path,
        std::function<void(std::shared_ptr<ValidPathInfo>)> success,
        std::function<void(std::exception_ptr exc)> failure) override;
    void nix::LocalStore::queryReferrers(const Path & path, PathSet & referrers) override;
    PathSet nix::LocalStore::queryValidDerivers(const Path & path) override;
    PathSet nix::LocalStore::queryDerivationOutputs(const Path & path) override;
    StringSet nix::LocalStore::queryDerivationOutputNames(const Path & path) override;
    Path nix::LocalStore::queryPathFromHashPart(const string & hashPart) override;
    PathSet nix::LocalStore::querySubstitutablePaths(const PathSet & paths) override;
    void nix::LocalStore::querySubstitutablePathInfos(const PathSet & paths,
        SubstitutablePathInfos & infos) override;
    void nix::LocalStore::addToStore(const ValidPathInfo & info, const ref<std::string> & nar,
        bool repair, bool dontCheckSigs,
        std::shared_ptr<FSAccessor> accessor) override;
    Path nix::LocalStore::addToStore(const string & name, const Path & srcPath,
        bool recursive, HashType hashAlgo,
        PathFilter & filter, bool repair) override;
    Path nix::LocalStore::addToStoreFromDump(const string & dump, const string & name,
        bool recursive = true, HashType hashAlgo = htSHA256, bool repair = false);
    Path nix::LocalStore::addTextToStore(const string & name, const string & s,
        const PathSet & references, bool repair) override;
    void nix::LocalStore::buildPaths(const PathSet & paths, BuildMode buildMode) override;
    BuildResult nix::LocalStore::buildDerivation(const Path & drvPath, const BasicDerivation & drv,BuildMode buildMode) override;
    void nix::LocalStore::ensurePath(const Path & path) override;
    void nix::LocalStore::addTempRoot(const Path & path) override;
    void nix::LocalStore::addIndirectRoot(const Path & path) override;
    void nix::LocalStore::syncWithGC() override;
    Roots nix::LocalStore::findRoots() override;
    void nix::LocalStore::collectGarbage(const GCOptions & options, GCResults & results) override;
    void nix::LocalStore::optimiseStore(OptimiseStats & stats);
    void nix::LocalStore::optimiseStore() override;
    void nix::LocalStore::optimisePath(const Path & path);
    bool nix::LocalStore::verifyStore(bool checkContents, bool repair) override;
    void nix::LocalStore::registerValidPath(const ValidPathInfo & info);
    void nix::LocalStore::registerValidPaths(const ValidPathInfos & infos);
    void nix::LocalStore::vacuumDB();
    void nix::LocalStore::repairPath(const Path & path);
    void nix::LocalStore::addSignatures(const Path & storePath, const StringSet & sigs) override;
};
typedef std::pair<dev_t, ino_t> nix::Inode;
typedef set<nix::Inode> nix::InodesSeen;
void nix::canonicalisePathMetaData(const Path & path, uid_t fromUid, nix::InodesSeen & inodesSeen);
void nix::canonicalisePathMetaData(const Path & path, uid_t fromUid);
void nix::canonicaliseTimestampAndPermissions(const Path & path);
MakeError(PathInUse, Error);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


nix::Value* findAlongAttrPath(EvalState& state, const string& attrPath, Bindings& autoArgs, Value& vIn);

constructor nix::Attr(Symbol name, Value* value, Pos* pos);
constructor nix::Attr();
bool nix::Attr::operator<(const Attr& a) const;

size_t           nix::Bindings::size() const;
bool             nix::Bindings::empty();
void             nix::Bindings::push_back(const Attr& attr);
iterator         nix::Bindings::find(const Symbol& name);
iterator         nix::Bindings::begin();
iterator         nix::Bindings::end();
Attr&            nix::Bindings::operator[](size_t pos);
void             nix::Bindings::sort();
size_t           nix::Bindings::capacity();
std::vector<const Attr*> nix::Bindings::lexicographicOrder() const;

bool      nix::Store::parseAutoArgs(Strings::iterator& i, const Strings::iterator& argsEnd, std::map<string, string>& res);
Bindings* nix::Store::evalAutoArgs(EvalState& state, std::map<string, string>& in);
bool      nix::Store::parseSearchPathArg(Strings::iterator& i, const Strings::iterator& argsEnd, Strings& searchPath);
Path      nix::Store::lookupFileArg(EvalState& state, string s);

nix::LocalNoInlineNoReturn(void throwEvalError(const char * s, const Pos& pos));
nix::LocalNoInlineNoReturn(void throwTypeError(const char * s, const Value& v));
nix::LocalNoInlineNoReturn(void throwTypeError(const char * s, const Value& v, const Pos& pos));

void nix::EvalState::forceValue(Value& v, const Pos& pos);
void nix::EvalState::forceAttrs(Value& v);
void nix::EvalState::forceAttrs(Value& v, const Pos& pos);
void nix::EvalState::forceList(Value& v);
void nix::EvalState::forceList(Value& v, const Pos& pos);

nix::PrimOp::PrimOp(PrimOpFun fun, size_t arity, Symbol name);

Value nix::& mkString(Value& v, const string& s, const PathSet& context = PathSet());

void nix::copyContext(const Value& v, PathSet& context);

std::ostream& nix::operator << (std::ostream& str, const Value& v);

void initGC();

// -----------------------------------------------------------------------------

nix::EvalState::EvalState(const Strings & _searchPath, ref<Store> store);
nix::EvalState::~EvalState();
void nix::EvalState::addToSearchPath(const string & s);
SearchPath nix::EvalState::getSearchPath() { return searchPath; }
Path nix::EvalState::checkSourcePath(const Path & path);
Expr * nix::EvalState::parseExprFromFile(const Path & path);
Expr * nix::EvalState::parseExprFromFile(const Path & path, StaticEnv & staticEnv);
Expr * nix::EvalState::parseExprFromString(const string & s, const Path & basePath, StaticEnv & staticEnv);
Expr * nix::EvalState::parseExprFromString(const string & s, const Path & basePath);
void nix::EvalState::evalFile(const Path & path, Value & v);
void nix::EvalState::resetFileCache();
Path nix::EvalState::findFile(const string & path);
Path nix::EvalState::findFile(SearchPath & searchPath, const string & path, const Pos & pos = noPos);
std::pair<bool, std::string> nix::EvalState::resolveSearchPathElem(const SearchPathElem & elem);
void nix::EvalState::eval(Expr * e, Value & v);
bool nix::EvalState::evalBool(Env & env, Expr * e);
bool nix::EvalState::evalBool(Env & env, Expr * e, const Pos & pos);
void nix::EvalState::evalAttrs(Env & env, Expr * e, Value & v);
void nix::EvalState::forceValue(Value & v, const Pos & pos = noPos);
void nix::EvalState::forceValueDeep(Value & v);
NixInt nix::EvalState::forceInt(Value & v, const Pos & pos);
NixFloat nix::EvalState::forceFloat(Value & v, const Pos & pos);
bool nix::EvalState::forceBool(Value & v, const Pos & pos);
void nix::EvalState::forceAttrs(Value & v);
void nix::EvalState::forceAttrs(Value & v, const Pos & pos);
void nix::EvalState::forceList(Value & v);
void nix::EvalState::forceList(Value & v, const Pos & pos);
void nix::EvalState::forceFunction(Value & v, const Pos & pos); // either lambda or primop
string nix::EvalState::forceString(Value & v, const Pos & pos = noPos);
string nix::EvalState::forceString(Value & v, PathSet & context, const Pos & pos = noPos);
string nix::EvalState::forceStringNoCtx(Value & v, const Pos & pos = noPos);
bool nix::EvalState::isDerivation(Value & v);
string nix::EvalState::coerceToString(const Pos & pos, Value & v, PathSet & context, bool coerceMore = false, bool copyToStore = true);
string nix::EvalState::copyPathToStore(PathSet & context, const Path & path);
Path nix::EvalState::coerceToPath(const Pos & pos, Value & v, PathSet & context);
Value & nix::EvalState::getBuiltin(const string & name);
Value * nix::EvalState::lookupVar(Env * env, const ExprVar & var, bool noEval);
Expr * nix::EvalState::parse(const char * text, const Path & path, const Path & basePath, StaticEnv & staticEnv);
bool nix::EvalState::eqValues(Value & v1, Value & v2);
bool nix::EvalState::isFunctor(Value & fun);
void nix::EvalState::callFunction(Value & fun, Value & arg, Value & v, const Pos & pos);
void nix::EvalState::callPrimOp(Value & fun, Value & arg, Value & v, const Pos & pos);
void nix::EvalState::autoCallFunction(Bindings & args, Value & fun, Value & res);
Value * nix::EvalState::allocValue();
Env & nix::EvalState::allocEnv(unsigned int size);
Value * nix::EvalState::allocAttr(Value & vAttrs, const Symbol & name);
Bindings * nix::EvalState::allocBindings(Bindings::size_t capacity);
void nix::EvalState::mkList(Value & v, unsigned int length);
void nix::EvalState::mkAttrs(Value & v, unsigned int capacity);
void nix::EvalState::mkThunk_(Value & v, Expr * expr);
void nix::EvalState::mkPos(Value & v, Pos * pos);
void nix::EvalState::concatLists(Value & v, unsigned int nrLists, Value * * lists, const Pos & pos);
void nix::EvalState::printStats();
void nix::EvalState::realiseContext(const PathSet & context);
string nix::showType(const Value & v);
Path nix::resolveExprPath(Path path);
nix::InvalidPathError::InvalidPathError(const Path & path);
nix::InvalidPathError::~InvalidPathError();

nix::DrvInfo::DrvInfo(EvalState & state);
nix::DrvInfo::DrvInfo(EvalState & state, const string & name, const string & attrPath, const string & system, Bindings * attrs);
string nix::DrvInfo::queryDrvPath();
string nix::DrvInfo::queryOutPath();
string nix::DrvInfo::queryOutputName();
Outputs nix::DrvInfo::queryOutputs(bool onlyOutputsToInstall = false);
StringSet nix::DrvInfoqueryMetaNames();
Value * nix::DrvInfo::queryMeta(const string & name);
string nix::DrvInfo::queryMetaString(const string & name);
NixInt nix::DrvInfo::queryMetaInt(const string & name, NixInt def);
NixFloat nix::DrvInfo::queryMetaFloat(const string & name, NixFloat def);
bool nix::DrvInfo::queryMetaBool(const string & name, bool def);
void nix::DrvInfo::setMeta(const string & name, Value * v);
void nix::DrvInfo::setDrvPath(const string & s);
void nix::DrvInfo::setOutPath(const string & s);
void nix::DrvInfo::setFailed();
bool nix::DrvInfo::hasFailed();
bool nix::DrvInfo::getDerivation(EvalState & state, Value & v, DrvInfo & drv, bool ignoreAssertionFailures);
void nix::DrvInfo::getDerivations(EvalState & state, Value & v, const string & pathPrefix, Bindings & autoArgs, DrvInfos & drvs, bool ignoreAssertionFailures);

void nix::parseJSON(EvalState & state, const string & s, Value & v);

nix::DrvName::DrvName();
nix::DrvName::DrvName(const string & s);
bool nix::DrvName::matches(DrvName & n);
int nix::compareVersions(const string & v1, const string & v2);
DrvNames nix::drvNamesFromArgs(const Strings & opArgs);

nix::ExprAttrs::AttrDef::(Expr * e, const Pos & pos, bool inherited=false);
nix::ExprAttrs::AttrDef::AttrDef();
nix::ExprAttr::DynamicAttrDef(Expr * nameExpr, Expr * valueExpr, const Pos & pos);
nix::ExprAttrs::ExprAttrs();
nix::ExprList::ExprList();
nix::Formal::Formal(const Symbol & name, Expr * def);
nix::ExprLambda::ExprLambda(const Pos & pos, const Symbol & arg, bool matchAttrs, Formals * formals, Expr * body);
void nix::ExprLambda::setName(Symbol & name);
string nix::ExprLambda::showNamePos() const;
ExprLet::ExprLet(ExprAttrs * attrs, Expr * body);
ExprWith::ExprWith(const Pos & pos, Expr * attrs, Expr * body);
nix::ExprIf::(Expr * cond, Expr * then, Expr * else_);
nix::ExprAssert::(const Pos & pos, Expr * cond, Expr * body);
nix::ExprOpNot::ExprOpNot(Expr * e);
nix::ExprConcatStrings::ExprConcatStrings(const Pos & pos, bool forceString, vector<Expr *> * es);
nix::ExprPos::ExprPos(const Pos & pos);
nix::StaticEnv::StaticEnv(bool isWith, const StaticEnv * up);

nix::RegisterPrimOp::RegisterPrimOp(std::string name, size_t arity, PrimOpFun fun);

nix::Symbol::Symbol();
nix::Symbol::operator == (const Symbol & s2) const;
nix::Symbol::operator != (const Symbol & s2) const;
nix::Symbol::operator < (const Symbol & s2) const;
bool nix::Symbol::operator const string & () const;
bool nix::Symbol::set() const;
bool nix::Symbol::empty() const;
Symbol nix::SymbolTable::create(const string & s);
unsigned int nix::SymbolTable::size() const;
size_t nix::SymbolTable::totalSize() const;

void nix::printValueAsJSON(EvalState & state, bool strict, Value & v, JSONPlaceholder & out, PathSet & context);
void nix::printValueAsJSON(EvalState & state, bool strict,Value & v, std::ostream & str, PathSet & context);

void nix::printValueAsXML(EvalState & state, bool strict, bool location, Value & v, std::ostream & out, PathSet & context);

std::ostream & nix::operator << (std::ostream & str, const ExternalValueBase & v);
bool nix::Value::isList() const;
Value * * nix::listElems();
const Value * const * nix::Value::listElems() const;
unsigned int nix::Value::listSize() const;
static void nix::clearValue(Value & v);
static void nix::mkInt(Value & v, NixInt n);
static void nix::mkFloat(Value & v, NixFloat n);
static void nix::mkBool(Value & v, bool b);
static void nix::mkNull(Value & v);
static void nix::mkApp(Value & v, Value & left, Value & right);
static void nix::mkStringNoCopy(Value & v, const char * s);
static void mkString(Value & v, const Symbol & s);
void mkString(Value & v, const char * s);
static void nix::mkPathNoCopy(Value & v, const char * s);
void nix::mkPath(Value & v, const char * s);
size_t nix::valueSize(Value & v);
