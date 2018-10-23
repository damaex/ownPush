package net.damaex.OwnPush;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;

import androidx.work.Constraints;
import androidx.work.ExistingWorkPolicy;
import androidx.work.NetworkType;
import androidx.work.OneTimeWorkRequest;
import androidx.work.WorkManager;
import androidx.work.Worker;
import androidx.work.WorkerParameters;

public class OwnPushRunChecker extends Worker {
    private static String UNIQUE_WORK_NAME = "OwnPushWorker";

    public OwnPushRunChecker(@NonNull Context appContext, @NonNull WorkerParameters workerParams) {
        super(appContext, workerParams);
    }

    @NonNull
    @Override
    public Result doWork() {
        Log.d("ownPush", "RunChecker started");
        Constraints constraints = new Constraints.Builder().setRequiredNetworkType(NetworkType.CONNECTED).build();
        OneTimeWorkRequest oneTimeWorkRequest = new OneTimeWorkRequest.Builder(OwnPushRunner.class).setConstraints(constraints).build();
        WorkManager.getInstance().beginUniqueWork(UNIQUE_WORK_NAME, ExistingWorkPolicy.KEEP, oneTimeWorkRequest).enqueue();

        return Result.SUCCESS;
    }
}
