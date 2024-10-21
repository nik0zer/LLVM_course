#include "../sim.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

int main() {
    LLVMContext context;
    Module *module = new Module("app2.c", context);
    IRBuilder<> builder(context);


    // declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef)
    // local_unnamed_addr #1
    auto *int32Type = Type::getInt32Ty(context);
    auto *int8Type = Type::getInt8Ty(context);
    auto *voidType = Type::getVoidTy(context);
    auto *int64Type = Type::getInt64Ty(context);
    auto *vecType = StructType::create(context, {int64Type, int64Type}, "struct.Vector");
    auto *objType = StructType::create(context, {vecType, vecType, int32Type}, "struct.Object");

    ArrayRef<Type *> simPutPixelParamTypes = {Type::getInt32Ty(context),
                                                Type::getInt32Ty(context),
                                                Type::getInt32Ty(context)};
    
    auto *simPutPixelType = FunctionType::get(voidType, simPutPixelParamTypes, false);
    auto simPutPixelFunc = module->getOrInsertFunction("simPutPixel", simPutPixelType);

    // declare void @simFlush(...) local_unnamed_addr #1
    auto *simFlushType = FunctionType::get(voidType, false);
    auto simFlushFunc = module->getOrInsertFunction("simFlush", simFlushType);

    auto *simRandType = FunctionType::get(int32Type, {int32Type, int32Type}, false);
    auto simRandFunc = module->getOrInsertFunction("simRand", simRandType);

    auto *simExitType = FunctionType::get(int32Type, false);
    auto simExitFunc = module->getOrInsertFunction("simExit", simExitType);

    // define dso_local void @app() local_unnamed_addr #0 {
    auto *appFuncType = FunctionType::get(builder.getVoidTy(), false);
    auto *appFunc = Function::Create(appFuncType, Function::ExternalLinkage, "app", module);

    auto* objPtrType = PointerType::getUnqual(objType);
    ArrayRef<llvm::Type*> paramTypes = {objPtrType};
    auto *argObjectsType = FunctionType::get(voidType, objPtrType, false);
    auto *voidFuncType = FunctionType::get(voidType, false);
    auto* int64PtrType = PointerType::getUnqual(int64Type);
    auto* int8PtrType = PointerType::getUnqual(int8Type);
    auto *drawObjectsFunc = 
        Function::Create(argObjectsType, Function::ExternalLinkage, "draw_objects", module);
    auto *setStartPositionsFunc = 
        Function::Create(argObjectsType, Function::InternalLinkage, "set_start_positions", module);
    auto *clearDrawFunc = 
        Function::Create(voidFuncType, Function::InternalLinkage, "clear_draw", module);
    auto *wallReflectionFunc = 
        Function::Create(argObjectsType, Function::InternalLinkage, "wall_reflection", module);
    auto *updatePosFunc = 
        Function::Create(argObjectsType, Function::InternalLinkage, "update_pos", module);

    {
        auto *BBApp0 = BasicBlock::Create(context, "", appFunc);
        auto *BBApp1 = BasicBlock::Create(context, "", appFunc);
        auto *BBApp2 = BasicBlock::Create(context, "", appFunc);
        auto *BBApp3 = BasicBlock::Create(context, "", appFunc);

        builder.SetInsertPoint(BBApp0);
        auto *objVal = builder.CreateAlloca(objType);
        builder.CreateCall(setStartPositionsFunc, objVal);
        builder.CreateBr(BBApp1);
        builder.SetInsertPoint(BBApp1);
        builder.CreateCall(wallReflectionFunc, objVal);
        builder.CreateCall(updatePosFunc, objVal);
        builder.CreateCall(drawObjectsFunc, objVal);
        auto *exitVal = builder.CreateCall(simExitFunc);
        auto *icmpVal0 = builder.CreateICmpEQ(exitVal, builder.getInt32(1));
        builder.CreateCondBr(icmpVal0, BBApp2, BBApp3);
        builder.SetInsertPoint(BBApp2);
        builder.CreateRetVoid();
        builder.SetInsertPoint(BBApp3);
        builder.CreateCall(simFlushFunc);
        builder.CreateBr(BBApp1);


        
        auto *BBDraw0 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw1 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw2 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw3 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw4 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw5 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw6 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw7 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw8 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw9 = BasicBlock::Create(context, "", drawObjectsFunc);
        auto *BBDraw10 = BasicBlock::Create(context, "", drawObjectsFunc);

        builder.SetInsertPoint(BBDraw0);
        builder.CreateBr(BBDraw1);
        builder.SetInsertPoint(BBDraw1);
        auto *drawObjPHI1 = builder.CreatePHI(int64Type, 2);
        drawObjPHI1->addIncoming(llvm::ConstantInt::get(int64Type, 0), BBDraw0);
        auto *truncVal = builder.CreateTrunc(drawObjPHI1, int32Type);
        builder.CreateBr(BBDraw4);
        builder.SetInsertPoint(BBDraw2);
        auto *nswAddVal = builder.CreateNSWAdd(drawObjPHI1, ConstantInt::get(int64Type, 1));
        drawObjPHI1->addIncoming(nswAddVal, BBDraw2);
        auto *icmpVal1 = builder.CreateICmpEQ(nswAddVal, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal1, BBDraw3, BBDraw1);
        builder.SetInsertPoint(BBDraw3);
        auto* elementPtr0 = builder.CreateInBoundsGEP(objType, drawObjectsFunc->getArg(0), 
                {
                    ConstantInt::get(int64Type, 0),
                    ConstantInt::get(int32Type, 0),
                    ConstantInt::get(int32Type, 0)
                });

        auto* elementPtr1 = builder.CreateInBoundsGEP(objType, drawObjectsFunc->getArg(0), 
                {
                    ConstantInt::get(int64Type, 0),
                    ConstantInt::get(int32Type, 0),
                    ConstantInt::get(int32Type, 1)
                });

        auto* elementPtr2 = builder.CreateInBoundsGEP(objType, drawObjectsFunc->getArg(0), 
                {
                    ConstantInt::get(int64Type, 0),
                    ConstantInt::get(int32Type, 2)
                });

        builder.CreateBr(BBDraw5);
        builder.SetInsertPoint(BBDraw4);
        auto *drawObjPHI2 = builder.CreatePHI(int64Type, 2);
        drawObjPHI2->addIncoming(ConstantInt::get(int64Type, 0), BBDraw1);
        auto *truncVal1 = builder.CreateTrunc(drawObjPHI2, int32Type);
        builder.CreateCall(simPutPixelFunc, {truncVal, truncVal1, ConstantInt::get(int32Type, 0)});
        auto *nswAddVal1 = builder.CreateNSWAdd(drawObjPHI2, ConstantInt::get(int64Type, 1));
        drawObjPHI2->addIncoming(nswAddVal1, BBDraw4);
        auto *icmpVal2 = builder.CreateICmpEQ(nswAddVal1, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal2, BBDraw2, BBDraw4);
        builder.SetInsertPoint(BBDraw5);
        auto *drawObjPHI3 = builder.CreatePHI(int64Type, 2);
        drawObjPHI3->addIncoming(ConstantInt::get(int64Type, 0), BBDraw3);
        auto *truncVal2 = builder.CreateTrunc(drawObjPHI3, int32Type);
        builder.CreateBr(BBDraw8);
        builder.SetInsertPoint(BBDraw6);
        builder.CreateRetVoid();
        builder.SetInsertPoint(BBDraw7);
        auto *nswAddVal2 = builder.CreateNSWAdd(drawObjPHI3, ConstantInt::get(int64Type, 1));
        drawObjPHI3->addIncoming(nswAddVal2, BBDraw7);
        auto *icmpVal3 = builder.CreateICmpEQ(nswAddVal2, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal3, BBDraw6, BBDraw5);
        builder.SetInsertPoint(BBDraw8);
        auto *drawObjPHI4 = builder.CreatePHI(int64Type, 2);
        drawObjPHI4->addIncoming(ConstantInt::get(int64Type, 0), BBDraw5);
        auto *loadVal1 = builder.CreateAlignedLoad(int64Type, elementPtr0, Align(8));
        auto *sdivVal1 = builder.CreateSDiv(loadVal1, ConstantInt::get(int64Type, 10));
        auto *subVal1 = builder.CreateSub(sdivVal1, drawObjPHI3);
        auto *mulVal1 = builder.CreateMul(subVal1, subVal1);
        auto *loadVal2 = builder.CreateAlignedLoad(int64Type, elementPtr1, Align(8));
        auto *sdivVal2 = builder.CreateSDiv(loadVal2, ConstantInt::get(int64Type, 10));
        auto *nswSubVal1 = builder.CreateNSWSub(sdivVal2, drawObjPHI4);
        auto *mulVal2 = builder.CreateMul(nswSubVal1, nswSubVal1);
        auto *addVal1 = builder.CreateAdd(mulVal2, mulVal1);
        auto *icmpVal4 = builder.CreateICmpULT(addVal1, ConstantInt::get(int64Type, 401));
        builder.CreateCondBr(icmpVal4, BBDraw9, BBDraw10);
        builder.SetInsertPoint(BBDraw9);
        auto *truncVal3 = builder.CreateTrunc(drawObjPHI4, int32Type);
        auto *loadVal3 = builder.CreateAlignedLoad(int32Type, elementPtr2, Align(8));
        builder.CreateCall(simPutPixelFunc, {truncVal2, truncVal3, loadVal3});
        builder.CreateBr(BBDraw10);
        builder.SetInsertPoint(BBDraw10);
        auto *nswAddVal3 = builder.CreateNSWAdd(drawObjPHI4, ConstantInt::get(int64Type, 1));
        drawObjPHI4->addIncoming(nswAddVal3, BBDraw10);
        auto *icmpVal5 = builder.CreateICmpEQ(nswAddVal3, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal5, BBDraw7, BBDraw8);
    }

    {
        auto *BB0 = BasicBlock::Create(context, "", wallReflectionFunc);
        auto *BB3 = BasicBlock::Create(context, "", wallReflectionFunc);
        auto *BB8 = BasicBlock::Create(context, "", wallReflectionFunc);
        auto *BB12 = BasicBlock::Create(context, "", wallReflectionFunc);
        auto *BB17 = BasicBlock::Create(context, "", wallReflectionFunc);
        auto *BB21 = BasicBlock::Create(context, "", wallReflectionFunc);

        builder.SetInsertPoint(BB0);
        auto *icmpVal1 = builder.CreateICmpEQ(wallReflectionFunc->getArg(0), ConstantPointerNull::get(objPtrType));
        builder.CreateCondBr(icmpVal1, BB21, BB3);
        builder.SetInsertPoint(BB3);
        auto* elementPtr0 = builder.CreateInBoundsGEP(objType, wallReflectionFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 0),
                ConstantInt::get(int32Type, 0)
            });
        auto *loadVal1 = builder.CreateAlignedLoad(int64Type, elementPtr0, Align(8));
        auto *addVal1 = builder.CreateAdd(loadVal1, ConstantInt::get(int64Type, -4620));
        auto *icmpVal2 = builder.CreateICmpULT(addVal1, ConstantInt::get(int64Type, -4110));
        builder.CreateCondBr(icmpVal2, BB8, BB12);
        builder.SetInsertPoint(BB8);
        auto* elementPtr1 = builder.CreateInBoundsGEP(objType, wallReflectionFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 1),
                ConstantInt::get(int32Type, 0)
            });
        auto *loadVal2 = builder.CreateAlignedLoad(int64Type, elementPtr1, Align(8));
        auto *nswSubVal1 = builder.CreateNSWSub(ConstantInt::get(int64Type, 0), loadVal2);
        builder.CreateAlignedStore(nswSubVal1, elementPtr1, Align(8));
        builder.CreateBr(BB12);
        builder.SetInsertPoint(BB12);
        auto* elementPtr2 = builder.CreateInBoundsGEP(objType, wallReflectionFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 0),
                ConstantInt::get(int32Type, 1)
            });
        auto *loadVal3 = builder.CreateAlignedLoad(int64Type, elementPtr2, Align(8));
        auto *addVal2 = builder.CreateAdd(loadVal3, ConstantInt::get(int64Type, -4620));
        auto *icmpVal3 = builder.CreateICmpULT(addVal2, ConstantInt::get(int64Type, -4110));
        builder.CreateCondBr(icmpVal3, BB17, BB21);
        builder.SetInsertPoint(BB17);
        auto* elementPtr3 = builder.CreateInBoundsGEP(objType, wallReflectionFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 1),
                ConstantInt::get(int32Type, 1)
            });
        auto *loadVal4 = builder.CreateAlignedLoad(int64Type, elementPtr3, Align(8));
        auto *nswSubVal2 = builder.CreateNSWSub(ConstantInt::get(int64Type, 0), loadVal4);
        builder.CreateAlignedStore(nswSubVal2, elementPtr3, Align(8));
        builder.CreateBr(BB21);
        builder.SetInsertPoint(BB21);
        builder.CreateRetVoid();
    }

    {
        auto *BB0 = BasicBlock::Create(context, "", updatePosFunc);
        auto *BB3 = BasicBlock::Create(context, "", updatePosFunc);
        auto *BB11 = BasicBlock::Create(context, "", updatePosFunc);

        builder.SetInsertPoint(BB0);
        auto *icmpVal1 = builder.CreateICmpEQ(updatePosFunc->getArg(0), ConstantPointerNull::get(objPtrType));
        builder.CreateCondBr(icmpVal1, BB11, BB3);
        builder.SetInsertPoint(BB3);
        auto* elementPtr = builder.CreateInBoundsGEP(objType, updatePosFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 1),
                ConstantInt::get(int32Type, 0)
            });
        auto *vecType = VectorType::get(int64Type, 2, false);    
        auto *vecPtrType = PointerType::getUnqual(vecType);
        auto *bitcastVal1 = builder.CreateBitCast(elementPtr, vecPtrType);
        auto *loadVal1 = builder.CreateAlignedLoad(vecType, bitcastVal1, Align(8));
        auto *bitcastVal2 = builder.CreateBitCast(updatePosFunc->getArg(0), vecPtrType);
        auto *loadVal2 = builder.CreateAlignedLoad(vecType, bitcastVal2, Align(8));
        auto *nswAddVal1 = builder.CreateNSWAdd(loadVal2, loadVal1);
        auto *bitcastVal3 = builder.CreateBitCast(updatePosFunc->getArg(0), vecPtrType);
        builder.CreateAlignedStore(nswAddVal1, bitcastVal3, Align(8));
        builder.CreateBr(BB11);
        builder.SetInsertPoint(BB11);
        builder.CreateRetVoid();
    }

    {
        auto *BB0 = BasicBlock::Create(context, "", clearDrawFunc);
        auto *BB1 = BasicBlock::Create(context, "", clearDrawFunc);
        auto *BB4 = BasicBlock::Create(context, "", clearDrawFunc);
        auto *BB5 = BasicBlock::Create(context, "", clearDrawFunc);
        auto *BB8 = BasicBlock::Create(context, "", clearDrawFunc);

        builder.SetInsertPoint(BB0);
        builder.CreateBr(BB1);
        builder.SetInsertPoint(BB1);
        auto *phiVal1 = builder.CreatePHI(int64Type, 2);
        phiVal1->addIncoming(ConstantInt::get(int64Type, 0), BB0);
        auto *truncVal1 = builder.CreateTrunc(phiVal1, int32Type);
        builder.CreateBr(BB8);
        builder.SetInsertPoint(BB4);
        builder.CreateRetVoid();
        builder.SetInsertPoint(BB5);
        auto *nswAddVal1 = builder.CreateNSWAdd(phiVal1, ConstantInt::get(int64Type, 1));
        auto *icmpVal1 = builder.CreateICmpEQ(nswAddVal1, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal1, BB4, BB1);
        phiVal1->addIncoming(nswAddVal1, BB5);
        builder.SetInsertPoint(BB8);
        auto *phiVal2 = builder.CreatePHI(int64Type, 2);
        phiVal2->addIncoming(ConstantInt::get(int64Type, 0), BB1);
        auto *truncVal2 = builder.CreateTrunc(phiVal2, int32Type);
        builder.CreateCall(simPutPixelFunc, {truncVal1, truncVal2, ConstantInt::get(int32Type, 0)});
        auto *nswAddVal2 = builder.CreateNSWAdd(phiVal2, ConstantInt::get(int64Type, 1));
        phiVal2->addIncoming(nswAddVal2, BB8);
        auto *icmpVal2 = builder.CreateICmpEQ(nswAddVal2, ConstantInt::get(int64Type, SIM_X_SIZE));
        builder.CreateCondBr(icmpVal2, BB5, BB8);
    }

    {
        auto *BB0 =  BasicBlock::Create(context, "", setStartPositionsFunc);

        builder.SetInsertPoint(BB0);
        auto* elementPtr0 = builder.CreateInBoundsGEP(objType, setStartPositionsFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 1),
                ConstantInt::get(int32Type, 0)
            });

        auto* elementPtr1 = builder.CreateInBoundsGEP(objType, setStartPositionsFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 1),
                ConstantInt::get(int32Type, 1)
            });

        auto *bitcastVal1 = builder.CreateBitOrPointerCast(elementPtr0, int8PtrType);
        builder.CreateMemSet(bitcastVal1, ConstantInt::get(int8Type, 0), ConstantInt::get(int64Type, 16), Align(8));
        auto *callVal1 = builder.CreateCall(simRandFunc, {ConstantInt::get(int32Type, 500), 
        ConstantInt::get(int32Type, 4620)});
        auto *sextVal1 = builder.CreateSExt(callVal1, int64Type);
        auto* elementPtr2 = builder.CreateInBoundsGEP(objType, setStartPositionsFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 0),
                ConstantInt::get(int32Type, 0)
            });
        builder.CreateAlignedStore(sextVal1, elementPtr2, Align(8));
        auto *callVal2 = builder.CreateCall(simRandFunc, {ConstantInt::get(int32Type, 500), 
        ConstantInt::get(int32Type, 4619)});
        auto *sextVal2 = builder.CreateSExt(callVal2, int64Type);
        auto* elementPtr3 = builder.CreateInBoundsGEP(objType, setStartPositionsFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 0),
                ConstantInt::get(int32Type, 1)
            });
        builder.CreateAlignedStore(sextVal2, elementPtr3, Align(8));
        auto *callVal3 = builder.CreateCall(simRandFunc, {ConstantInt::get(int32Type, -200), 
        ConstantInt::get(int32Type, 200)});
        auto *sextVal3 = builder.CreateSExt(callVal3, int64Type);
        builder.CreateAlignedStore(sextVal3, elementPtr0, Align(8));
        auto *callVal4 = builder.CreateCall(simRandFunc, {ConstantInt::get(int32Type, -200), 
        ConstantInt::get(int32Type, 200)});
        auto *sextVal4 = builder.CreateSExt(callVal4, int64Type);
        builder.CreateAlignedStore(sextVal4, elementPtr1, Align(8));
        auto *callVal5 = builder.CreateCall(simRandFunc, {ConstantInt::get(int32Type, 0), 
        ConstantInt::get(int32Type, __INT32_MAX__)});
        auto* elementPtr4 = builder.CreateInBoundsGEP(objType, setStartPositionsFunc->getArg(0), 
            {
                ConstantInt::get(int64Type, 0),
                ConstantInt::get(int32Type, 2)
            });
        builder.CreateAlignedStore(callVal5, elementPtr4, Align(8));
        builder.CreateRetVoid();

    }

    module->print(outs(), nullptr);

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
        if (fnName == "simPutPixel") {
        return reinterpret_cast<void *>(simPutPixel);
        }
        if (fnName == "simFlush") {
        return reinterpret_cast<void *>(simFlush);
        }
        if (fnName == "simRand") {
        return reinterpret_cast<void *>(simRand);
        }
        if (fnName == "simExit") {
        return reinterpret_cast<void *>(simExit);
        }
        return nullptr;
    });
    ee->finalizeObject();

    simInit();
    ArrayRef<GenericValue> noargs;
    GenericValue v = ee->runFunction(appFunc, noargs);
    outs() << "[EE] Result: " << v.IntVal << "\n";
    return 0;

}