USE [MuOnline]
GO
/****** Object:  StoredProcedure [dbo].[WZ_CustomRanking]    Script Date: 03/07/2026 19:42:27 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[WZ_CustomRanking]
	@type SMALLINT
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @vipExpression NVARCHAR(200);
	DECLARE @vipJoin NVARCHAR(300);
	DECLARE @sql NVARCHAR(MAX);
	IF COL_LENGTH('dbo.MEMB_INFO', 'AccountLevel') IS NOT NULL
	BEGIN
		SET @vipExpression = N'ISNULL(MI.AccountLevel, 0)';
		SET @vipJoin = N' LEFT JOIN dbo.MEMB_INFO MI ON MI.memb___id = CH.AccountID';
	END
	ELSE
	BEGIN
		SET @vipExpression = N'0';
		SET @vipJoin = N'';
	END

	--===================================
	-- RANKING 0 - RESET DIARIO
	--===================================
	IF @type = 0
	BEGIN
		IF COL_LENGTH('dbo.Character', 'ResetDay') IS NULL
		BEGIN
			SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
			FROM dbo.Character;
			RETURN;
		END
		SET @sql = N'SELECT TOP 20 CH.Name AS VALUE1, ISNULL(CH.ResetDay,0) AS VALUE2, ' +
			N'CH.Class AS VALUE3, ' + @vipExpression +
			N' AS VALUE4 FROM dbo.Character CH' + @vipJoin +
			N' WHERE CH.CtlCode = 0' +
			N' ORDER BY CH.ResetDay DESC, CH.cLevel DESC';
		EXEC sys.sp_executesql @sql;
		RETURN;
	END

	--===================================
	-- RANKING 1 - RESET SEMANAL
	--===================================
	IF @type = 1
	BEGIN
		IF COL_LENGTH('dbo.Character', 'ResetWeek') IS NULL
		BEGIN
			SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
			FROM dbo.Character;
			RETURN;
		END
		SET @sql = N'SELECT TOP 20 CH.Name AS VALUE1, ISNULL(CH.ResetWeek,0) AS VALUE2, ' +
			N'CH.Class AS VALUE3, ' + @vipExpression +
			N' AS VALUE4 FROM dbo.Character CH' + @vipJoin +
			N' WHERE CH.CtlCode = 0' +
			N' ORDER BY CH.ResetWeek DESC, CH.cLevel DESC';
		EXEC sys.sp_executesql @sql;
		RETURN;
	END

	--===================================
	-- RANKING 2 - RESET MENSAL
	--===================================
	IF @type = 2
	BEGIN
		IF COL_LENGTH('dbo.Character', 'ResetMonth') IS NULL
		BEGIN
			SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
			FROM dbo.Character;
			RETURN;
		END
		SET @sql = N'SELECT TOP 20 CH.Name AS VALUE1, ISNULL(CH.ResetMonth,0) AS VALUE2, ' +
			N'CH.Class AS VALUE3, ' + @vipExpression +
			N' AS VALUE4 FROM dbo.Character CH' + @vipJoin +
			N' WHERE CH.CtlCode = 0' +
			N' ORDER BY CH.ResetMonth DESC, CH.cLevel DESC';
		EXEC sys.sp_executesql @sql;
		RETURN;
	END

	--===================================
	-- RANKING 3 - RESET GERAL
	--===================================
	IF @type = 3
	BEGIN
		IF COL_LENGTH('dbo.Character', 'ResetCount') IS NULL
		BEGIN
			SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
			FROM dbo.Character;
			RETURN;
		END
		SET @sql = N'SELECT TOP 20 CH.Name AS VALUE1, ISNULL(CH.ResetCount,0) AS VALUE2, ' +
			N'CH.Class AS VALUE3, ' + @vipExpression +
			N' AS VALUE4 FROM dbo.Character CH' + @vipJoin +
			N' WHERE CH.CtlCode = 0' +
			N' ORDER BY CH.ResetCount DESC, CH.cLevel DESC';
		EXEC sys.sp_executesql @sql;
		RETURN;
	END
	--===================================
	-- RANKING 4 - MASTER RESET
	--===================================
	IF @type = 4
	BEGIN
		DECLARE @masterResetColumn SYSNAME;
		SET @masterResetColumn =
			CASE
				WHEN COL_LENGTH('dbo.Character', 'MasterResetCount') IS NOT NULL THEN N'MasterResetCount'
				WHEN COL_LENGTH('dbo.Character', 'MasterReset') IS NOT NULL THEN N'MasterReset'
				ELSE NULL
			END;
		IF @masterResetColumn IS NULL
		BEGIN
			SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
			FROM dbo.Character;
			RETURN;
		END
		SET @sql = N'SELECT TOP 20 CH.Name AS VALUE1, ISNULL(CH.' + QUOTENAME(@masterResetColumn) +
			N',0) AS VALUE2, CH.Class AS VALUE3, ' + @vipExpression +
			N' AS VALUE4 FROM dbo.Character CH' + @vipJoin +
			N' WHERE CH.CtlCode = 0' +
			N' ORDER BY CH.' + QUOTENAME(@masterResetColumn) + N' DESC, CH.cLevel DESC';
		EXEC sys.sp_executesql @sql;
		RETURN;
	END

	SELECT TOP 0 Name AS VALUE1, 0 AS VALUE2, Class AS VALUE3, 0 AS VALUE4
	FROM dbo.Character;
END