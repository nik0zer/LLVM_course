#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <vector>

struct InstLogModPass : public llvm::PassInfoMixin<InstLogModPass>
{
    bool isLogger(llvm::StringRef name) {
      return name == "LOGGER";
    }
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
      if(isLogger(F.getName()))
      {
        return llvm::PreservedAnalyses::none();
      }

      llvm::LLVMContext &Ctx = F.getContext();
      llvm::IRBuilder<> builder(Ctx);
      llvm::Type *retType = llvm::Type::getVoidTy(Ctx);
      llvm::ArrayRef<llvm::Type *> ParamTypes = {
          builder.getInt8Ty()->getPointerTo()};
      llvm::FunctionType *LogFuncType =
          llvm::FunctionType::get(retType, ParamTypes, false);
      llvm::FunctionCallee LogFunc =
          F.getParent()->getOrInsertFunction("LOGGER", LogFuncType);
      
      for(auto &B : F)
      {
        for(auto &I : B)
        {
          if(llvm::isa<llvm::PHINode>(&I))
          {
            continue;
          }
          builder.SetInsertPoint(&I);
          for(auto &user : I.uses())
          {
            auto &&userInst = llvm::cast<llvm::Instruction>(user.getUser());
            if(llvm::isa<llvm::PHINode>(*userInst))
            {
              continue;
            }
            llvm::Value *log_data =
                builder.CreateGlobalStringPtr(std::string(userInst->getOpcodeName()) + " <- " + I.getOpcodeName());
            llvm::Value *args[] = {log_data};
            builder.CreateCall(LogFunc, args);
          }
        }
      }
      return llvm::PreservedAnalyses::none();
    };
    static bool isRequired() { return true; }
};

llvm::PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](llvm::PassBuilder &PB) {
    PB.registerOptimizerLastEPCallback([&](llvm::ModulePassManager &MPM, auto) {
        MPM.addPass(llvm::createModuleToFunctionPassAdaptor(InstLogModPass{}));
        return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "MyPlugin", "0.0.1", callback};
};

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
