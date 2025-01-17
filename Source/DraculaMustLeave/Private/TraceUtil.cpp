// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceUtil.h"



bool UTraceUtil::ConeTraceChannelLogic(UObject* WorldContextObject, const FVector& Start, const FVector& End,
    float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore,
        bool bIgnoreSelf, TArray<FHitResult>& OutHits, float& OutAngleRad, float& OutDistance, FVector& OutDirection)
{

   
    if (!WorldContextObject) return false;
    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return false;

    //Cone Basic Params
    OutDirection = (End - Start).GetSafeNormal();
    OutDistance = FVector::Distance(Start, End);

    //Gives the angle in Radians
    OutAngleRad = FMath::Atan(Radius / OutDistance);

    //Setting Up Collision Params
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SphereTrace), bTraceComplex);
    QueryParams.AddIgnoredActors(ActorsToIgnore);
    if (bIgnoreSelf) {
        if (const AActor* IgnoredActor = Cast<AActor>(WorldContextObject)) {
            QueryParams.AddIgnoredActor(IgnoredActor);
        }
    }

    //Set Shape & Cast
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
    bool bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, UEngineTypes::ConvertToCollisionChannel(TraceChannel), CollisionShape,
        QueryParams);
    if (!bHit) return false;

    
    //Compare DotProcuct (UpperDotLimit is 1)
    const float LowerDotLimit = FMath::Cos(OutAngleRad);
    for (int32 i = 0; i < OutHits.Num();)
    {
        AActor* HitActor = OutHits[i].GetActor();
        if (HitActor)
        {
            DrawDebugSphere(WorldContextObject->GetWorld(), HitActor->GetActorLocation(), 75.f, 12, FColor::Red, false );
           
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Channel: %d"), *HitActor->GetActorLabel(), OutHits[0].Component->GetCollisionObjectType());
        }
        
        const FVector& HitDir = (HitActor->GetActorLocation() - Start).GetSafeNormal();
        const float DotProduct = FVector::DotProduct(HitDir, OutDirection);

        //Remove Hits which do not fit in the shape
        if (DotProduct < LowerDotLimit || DotProduct > 1.f)
        {
            OutHits.RemoveAt(i);
        }
        else
        {
            ++i;
        }
    }
    return OutHits.IsEmpty() == false;
}

TArray<FHitResult> UTraceUtil::ConeTraceByChannel(UObject* WorldContextObject, const FVector& Start,
    const FVector& End, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore,
    EDrawDebugTrace::Type DrawDebugType, bool bIgnoreSelf, EConeDebugShape DebugShape, FColor TraceColor, FColor TraceHitColor,
    float DebugDuration = 2.f) {

    TArray<FHitResult> Hits;
    float AngleRad;
    float Distance;
    FVector Direction;

    bool HasHits = ConeTraceChannelLogic(WorldContextObject, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore,
        bIgnoreSelf, Hits, AngleRad, Distance, Direction);
    const FColor DebugColor = HasHits ? TraceHitColor : TraceColor;

    switch (DebugShape)
    {
        case EConeDebugShape::Cone:
        DrawDebugCone(WorldContextObject->GetWorld(), Start, Direction, Distance, AngleRad, AngleRad, 16, DebugColor,
       DrawDebugType == EDrawDebugTrace::Persistent, DrawDebugType == EDrawDebugTrace::ForOneFrame ? 0.f : DebugDuration);
        break;
    case EConeDebugShape::Circle:
        AActor* CameraActor = Cast<AActor>(WorldContextObject);
        if (CameraActor)
        {
            FVector CameraLocation = CameraActor->GetActorLocation();
            FVector CameraForwardVector = CameraActor->GetActorForwardVector();
    
            // Get the camera's rotation to adjust for pitch, yaw, and roll.
            FRotator CameraRotation = CameraActor->GetActorRotation();
    
            // Right vector should be perpendicular to the forward vector and the world up.
            FVector RightVector = CameraRotation.RotateVector(FVector::RightVector); // Use the Camera's right vector.
            FVector UpVector = CameraRotation.RotateVector(FVector::UpVector); // Use the Camera's up vector.
    
            // Position the circle at the base of the cone, a fixed distance in front of the camera
            FVector ConeBaseCenter = Start + Direction * 200.f;

            // Calculate the Base Radius based on distance to maintain visual consistency
            float BaseRadius = Radius * (200.f / Distance);
            UE_LOG(LogTemp, Warning, TEXT("Base Radius: %f, Distance: %f"), BaseRadius, Distance);
    
            // Draw the circle facing the camera, keeping the correct orientation
            DrawDebugCircle(WorldContextObject->GetWorld(), ConeBaseCenter, BaseRadius, 32, DebugColor,
                DrawDebugType == EDrawDebugTrace::Persistent, DrawDebugType == EDrawDebugTrace::ForOneFrame ? 0.f : DebugDuration, 
                0, 1.f, RightVector, UpVector, false);
        }
        break;
    }
    
   
    return Hits;
}